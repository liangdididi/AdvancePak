// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "AdvancePakLibrary.h"
#include "Json.h"


FString UAdvancePakLibrary::GetLongestPath(TArray<FPakInputPair>& FilesToAdd)
{
	FString LongestPath;
	int32 MaxNumDirectories = 0;

	for (int32 FileIndex = 0; FileIndex < FilesToAdd.Num(); FileIndex++)
	{
		FString& Filename = FilesToAdd[FileIndex].Dest;
		int32 NumDirectories = 0;
		for (int32 Index = 0; Index < Filename.Len(); Index++)
		{
			if (Filename[Index] == '/')
			{
				NumDirectories++;
			}
		}
		if (NumDirectories > MaxNumDirectories)
		{
			LongestPath = Filename;
			MaxNumDirectories = NumDirectories;
		}
	}
	return FPaths::GetPath(LongestPath) + TEXT("/");
}

FString UAdvancePakLibrary::GetCommonRootPath(TArray<FPakInputPair>& FilesToAdd)
{
	FString Root = GetLongestPath(FilesToAdd);
	for (int32 FileIndex = 0; FileIndex < FilesToAdd.Num() && Root.Len(); FileIndex++)
	{
		FString Filename(FilesToAdd[FileIndex].Dest);
		FString Path = FPaths::GetPath(Filename) + TEXT("/");
		int32 CommonSeparatorIndex = -1;
		int32 SeparatorIndex = Path.Find(TEXT("/"), ESearchCase::CaseSensitive);
		while (SeparatorIndex >= 0)
		{
			if (FCString::Strnicmp(*Root, *Path, SeparatorIndex + 1) != 0)
			{
				break;
			}
			CommonSeparatorIndex = SeparatorIndex;
			if (CommonSeparatorIndex + 1 < Path.Len())
			{
				SeparatorIndex = Path.Find(TEXT("/"), ESearchCase::CaseSensitive, ESearchDir::FromStart, CommonSeparatorIndex + 1);
			}
			else
			{
				break;
			}
		}
		if ((CommonSeparatorIndex + 1) < Root.Len())
		{
			Root = Root.Mid(0, CommonSeparatorIndex + 1);
		}
	}
	return Root;
}

FString UAdvancePakLibrary::SanitizeCacheKey(const TCHAR* PluginName, const TCHAR* VersionString, const TCHAR* PluginSpecificCacheKeySuffix)
{
	const TCHAR* CacheKey = *FString::Printf(TEXT("%s_%s_%s"), PluginName, VersionString, PluginSpecificCacheKeySuffix);
	FString Output;
	FString Input(CacheKey);
	int32 StartValid = 0;
	int32 NumValid = 0;

	for (int32 i = 0; i < Input.Len(); i++)
	{
		if (FChar::IsAlnum(Input[i]) || FChar::IsUnderscore(Input[i]))
		{
			NumValid++;
		}
		else
		{
			// Copy the valid range so far
			Output += Input.Mid(StartValid, NumValid);

			// Reset valid ranges
			StartValid = i + 1;
			NumValid = 0;

			// Replace the invalid character with a special string
			Output += FString::Printf(TEXT("$%x"), uint32(Input[i]));
		}
	}

	// Just return the input if the entire string was valid
	if (StartValid == 0 && NumValid == Input.Len())
	{
		return Input;
	}
	else if (NumValid > 0)
	{
		// Copy the remaining valid part
		Output += Input.Mid(StartValid, NumValid);
	}

	return Output;
}

bool UAdvancePakLibrary::PrepareCopyFileToPak(const FString& InMountPoint, const FPakInputPair& InFile, uint8*& InOutPersistentBuffer, int64& InOutBufferSize, FPakEntryPair& OutNewEntry, uint8*& OutDataToWrite, int64& OutSizeToWrite, const FKeyChain& InKeyChain)
{
	//TUniquePtr<FArchive> FileHandle(IFileManager::Get().CreateFileReader(*InFile.Source));
	TUniquePtr<FArchive> FileHandle = MakeUnique<FMemoryReader>(InFile.Bytes);
	bool bFileExists = FileHandle.IsValid();
	if (bFileExists)
	{
		const int64 FileSize = FileHandle->TotalSize();
		const int64 PaddedEncryptedFileSize = Align(FileSize, FAES::AESBlockSize);
		OutNewEntry.Filename = InFile.Dest.Mid(InMountPoint.Len());
		OutNewEntry.Info.Offset = 0; // Don't serialize offsets here.
		OutNewEntry.Info.Size = FileSize;
		OutNewEntry.Info.UncompressedSize = FileSize;
		OutNewEntry.Info.CompressionMethodIndex = 0;
		OutNewEntry.Info.SetEncrypted(InFile.bNeedEncryption);
		OutNewEntry.Info.SetDeleteRecord(false);

		if (InOutBufferSize < PaddedEncryptedFileSize)
		{
			InOutPersistentBuffer = (uint8*)FMemory::Realloc(InOutPersistentBuffer, PaddedEncryptedFileSize);
			InOutBufferSize = FileSize;
		}

		// Load to buffer
		FileHandle->Serialize(InOutPersistentBuffer, FileSize);

		{
			OutSizeToWrite = FileSize;
			if (InFile.bNeedEncryption)
			{
				for (int64 FillIndex = FileSize; FillIndex < PaddedEncryptedFileSize && InFile.bNeedEncryption; ++FillIndex)
				{
					// Fill the trailing buffer with bytes from file. Note that this is now from a fixed location
					// rather than a random one so that we produce deterministic results
					InOutPersistentBuffer[FillIndex] = InOutPersistentBuffer[(FillIndex - FileSize) % FileSize];
				}

				//Encrypt the buffer before writing it to disk
				check(InKeyChain.MasterEncryptionKey);
				FAES::EncryptData(InOutPersistentBuffer, PaddedEncryptedFileSize, InKeyChain.MasterEncryptionKey->Key);
				// Update the size to be written
				OutSizeToWrite = PaddedEncryptedFileSize;
				OutNewEntry.Info.SetEncrypted(true);
			}

			// Calculate the buffer hash value
			FSHA1::HashBuffer(InOutPersistentBuffer, FileSize, OutNewEntry.Info.Hash);
			OutDataToWrite = InOutPersistentBuffer;
		}
	}
	return bFileExists;
}

void UAdvancePakLibrary::FinalizeCopyCompressedFileToPak(FPakInfo& InPakInfo, const FCompressedFileBuffer& CompressedFile, FPakEntryPair& OutNewEntry)
{
	check(CompressedFile.TotalCompressedSize != 0);

	check(OutNewEntry.Info.CompressionBlocks.Num() == CompressedFile.CompressedBlocks.Num());
	check(OutNewEntry.Info.CompressionMethodIndex == InPakInfo.GetCompressionMethodIndex(CompressedFile.FileCompressionMethod));

	int64 TellPos = OutNewEntry.Info.GetSerializedSize(FPakInfo::PakFile_Version_Latest);
	const TArray<FPakCompressedBlock>& Blocks = CompressedFile.CompressedBlocks;
	for (int32 BlockIndex = 0, BlockCount = Blocks.Num(); BlockIndex < BlockCount; ++BlockIndex)
	{
		OutNewEntry.Info.CompressionBlocks[BlockIndex].CompressedStart = Blocks[BlockIndex].CompressedStart + TellPos;
		OutNewEntry.Info.CompressionBlocks[BlockIndex].CompressedEnd = Blocks[BlockIndex].CompressedEnd + TellPos;
	}
}

bool UAdvancePakLibrary::PrepareCopyCompressedFileToPak(const FString& InMountPoint, FPakInfo& Info, const FPakInputPair& InFile, const FCompressedFileBuffer& CompressedFile, FPakEntryPair& OutNewEntry, uint8*& OutDataToWrite, int64& OutSizeToWrite, const FKeyChain& InKeyChain)
{
	if (CompressedFile.TotalCompressedSize == 0)
	{
		return false;
	}

	OutNewEntry.Info.CompressionMethodIndex = Info.GetCompressionMethodIndex(CompressedFile.FileCompressionMethod);
	OutNewEntry.Info.CompressionBlocks.AddZeroed(CompressedFile.CompressedBlocks.Num());

	if (InFile.bNeedEncryption)
	{
		check(InKeyChain.MasterEncryptionKey);
		FAES::EncryptData(CompressedFile.CompressedBuffer.Get(), CompressedFile.TotalCompressedSize, InKeyChain.MasterEncryptionKey->Key);
	}

	//Hash the final buffer thats written
	FSHA1 Hash;
	Hash.Update(CompressedFile.CompressedBuffer.Get(), CompressedFile.TotalCompressedSize);
	Hash.Final();

	// Update file size & Hash
	OutNewEntry.Info.CompressionBlockSize = CompressedFile.FileCompressionBlockSize;
	OutNewEntry.Info.UncompressedSize = CompressedFile.OriginalSize;
	OutNewEntry.Info.Size = CompressedFile.TotalCompressedSize;
	Hash.GetHash(OutNewEntry.Info.Hash);

	//	Write the header, then the data
	OutNewEntry.Filename = InFile.Dest.Mid(InMountPoint.Len());
	OutNewEntry.Info.Offset = 0; // Don't serialize offsets here.
	OutNewEntry.Info.SetEncrypted(InFile.bNeedEncryption);
	OutNewEntry.Info.SetDeleteRecord(false);
	OutSizeToWrite = CompressedFile.TotalCompressedSize;
	OutDataToWrite = CompressedFile.CompressedBuffer.Get();
	//OutNewEntry.Info.Serialize(InPak,FPakInfo::PakFile_Version_Latest);	
	//InPak.Serialize(CompressedFile.CompressedBuffer.Get(), CompressedFile.TotalCompressedSize);

	return true;
}

void UAdvancePakLibrary::PrepareDeleteRecordForPak(const FString& InMountPoint, const FPakInputPair InDeletedFile, FPakEntryPair& OutNewEntry)
{
	OutNewEntry.Filename = InDeletedFile.Dest.Mid(InMountPoint.Len());
	OutNewEntry.Info.SetDeleteRecord(true);
}

bool UAdvancePakLibrary::BufferedCopyFile(FArchive& Dest, FArchive& Source, const FPakFile& PakFile, const FPakEntry& Entry, void* Buffer, int64 BufferSize, const FKeyChain& InKeyChain)
{
	// Align down
	BufferSize = BufferSize & ~(FAES::AESBlockSize - 1);
	int64 RemainingSizeToCopy = Entry.Size;
	while (RemainingSizeToCopy > 0)
	{
		const int64 SizeToCopy = FMath::Min(BufferSize, RemainingSizeToCopy);
		// If file is encrypted so we need to account for padding
		int64 SizeToRead = Entry.IsEncrypted() ? Align(SizeToCopy, FAES::AESBlockSize) : SizeToCopy;

		Source.Serialize(Buffer, SizeToRead);
		if (Entry.IsEncrypted())
		{
			const FNamedAESKey* Key = InKeyChain.MasterEncryptionKey;
			check(Key);
			FAES::DecryptData((uint8*)Buffer, SizeToRead, Key->Key);
		}
		Dest.Serialize(Buffer, SizeToCopy);
		RemainingSizeToCopy -= SizeToRead;
	}
	return true;
}

bool UAdvancePakLibrary::UncompressCopyFile(FArchive& Dest, FArchive& Source, const FPakEntry& Entry, uint8*& PersistentBuffer, int64& BufferSize, const FKeyChain& InKeyChain, const FPakFile& PakFile)
{
	if (Entry.UncompressedSize == 0)
	{
		return false;
	}

	// The compression block size depends on the bit window that the PAK file was originally created with. Since this isn't stored in the PAK file itself,
	// we can use FCompression::CompressMemoryBound as a guideline for the max expected size to avoid unncessary reallocations, but we need to make sure
	// that we check if the actual size is not actually greater (eg. UE-59278).
	FName EntryCompressionMethod = PakFile.GetInfo().GetCompressionMethod(Entry.CompressionMethodIndex);
	int32 MaxCompressionBlockSize = FCompression::CompressMemoryBound(EntryCompressionMethod, Entry.CompressionBlockSize);
	for (const FPakCompressedBlock& Block : Entry.CompressionBlocks)
	{
		MaxCompressionBlockSize = FMath::Max<int32>(MaxCompressionBlockSize, Block.CompressedEnd - Block.CompressedStart);
	}

	int64 WorkingSize = Entry.CompressionBlockSize + MaxCompressionBlockSize;
	if (BufferSize < WorkingSize)
	{
		PersistentBuffer = (uint8*)FMemory::Realloc(PersistentBuffer, WorkingSize);
		BufferSize = WorkingSize;
	}

	uint8* UncompressedBuffer = PersistentBuffer + MaxCompressionBlockSize;

	for (uint32 BlockIndex = 0, BlockIndexNum = Entry.CompressionBlocks.Num(); BlockIndex < BlockIndexNum; ++BlockIndex)
	{
		uint32 CompressedBlockSize = Entry.CompressionBlocks[BlockIndex].CompressedEnd - Entry.CompressionBlocks[BlockIndex].CompressedStart;
		uint32 UncompressedBlockSize = (uint32)FMath::Min<int64>(Entry.UncompressedSize - Entry.CompressionBlockSize * BlockIndex, Entry.CompressionBlockSize);
		Source.Seek(Entry.CompressionBlocks[BlockIndex].CompressedStart + (PakFile.GetInfo().HasRelativeCompressedChunkOffsets() ? Entry.Offset : 0));
		uint32 SizeToRead = Entry.IsEncrypted() ? Align(CompressedBlockSize, FAES::AESBlockSize) : CompressedBlockSize;
		Source.Serialize(PersistentBuffer, SizeToRead);

		if (Entry.IsEncrypted())
		{
			const FNamedAESKey* Key = InKeyChain.MasterEncryptionKey;
			check(Key);
			FAES::DecryptData(PersistentBuffer, SizeToRead, Key->Key);
		}

		if (!FCompression::UncompressMemory(EntryCompressionMethod, UncompressedBuffer, UncompressedBlockSize, PersistentBuffer, CompressedBlockSize))
		{
			return false;
		}
		Dest.Serialize(UncompressedBuffer, UncompressedBlockSize);
	}

	return true;
}

FRSAKeyHandle UAdvancePakLibrary::ParseRSAKeyFromJson(TSharedPtr<FJsonObject> InObj)
{
	TSharedPtr<FJsonObject> PublicKey = InObj->GetObjectField(TEXT("PublicKey"));
	TSharedPtr<FJsonObject> PrivateKey = InObj->GetObjectField(TEXT("PrivateKey"));

	FString PublicExponentBase64, PrivateExponentBase64, PublicModulusBase64, PrivateModulusBase64;

	if (PublicKey->TryGetStringField("Exponent", PublicExponentBase64)
		&& PublicKey->TryGetStringField("Modulus", PublicModulusBase64)
		&& PrivateKey->TryGetStringField("Exponent", PrivateExponentBase64)
		&& PrivateKey->TryGetStringField("Modulus", PrivateModulusBase64))
	{
		check(PublicModulusBase64 == PrivateModulusBase64);

		TArray<uint8> PublicExponent, PrivateExponent, Modulus;
		FBase64::Decode(PublicExponentBase64, PublicExponent);
		FBase64::Decode(PrivateExponentBase64, PrivateExponent);
		FBase64::Decode(PublicModulusBase64, Modulus);

		return FRSA::CreateKey(PublicExponent, PrivateExponent, Modulus);
	}
	else
	{
		return nullptr;
	}
}

bool UAdvancePakLibrary::ExtractPakFile(const TCHAR* InPakFilename, TArray<FPakInputPair>* OutEntries, const FKeyChain& InKeyChain)
{
	FPakFile PakFile(&FPlatformFileManager::Get().GetPlatformFile(), InPakFilename, false);
	if (PakFile.IsValid())
	{
		FArchive& PakReader = *PakFile.GetSharedReader(NULL);
		const int64 BufferSize = 8 * 1024 * 1024; // 8MB buffer for extracting
		void* Buffer = FMemory::Malloc(BufferSize);
		int64 CompressionBufferSize = 0;
		uint8* PersistantCompressionBuffer = NULL;
		int32 ErrorCount = 0;
		int32 FileCount = 0;
		int32 ExtractedCount = 0;

		FString SourceFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / FString("Visual") / PakFile.GetMountPoint().Replace(TEXT("../../../"), TEXT("")));

		for (FPakFile::FFileIterator It(PakFile, false); It; ++It, ++FileCount)
		{
			const FPakEntry& Entry = It.Info();

			PakReader.Seek(Entry.Offset);
			uint32 SerializedCrcTest = 0;
			FPakEntry EntryInfo;
			EntryInfo.Serialize(PakReader, PakFile.GetInfo().Version);
			if (EntryInfo == Entry)
			{
				FPakInputPair Input;
				FMemoryWriter MemoryFile(Input.Bytes);
				FArchive* FileHandle = &MemoryFile;

				if (Entry.CompressionMethodIndex == 0)
				{
					BufferedCopyFile(*FileHandle, PakReader, PakFile, Entry, Buffer, BufferSize, InKeyChain);
				}
				else
				{
					UncompressCopyFile(*FileHandle, PakReader, Entry, PersistantCompressionBuffer, CompressionBufferSize, InKeyChain, PakFile);
				}
				UE_LOG(LogTemp, Display, TEXT("Extracted \"%s\" Offset %d."), *It.Filename(), Entry.Offset);
				ExtractedCount++;

				Input.Source = SourceFilePath / It.Filename();
				FPaths::NormalizeFilename(Input.Source);

				Input.Dest = PakFile.GetMountPoint() / It.Filename();
				FPaths::NormalizeFilename(Input.Dest);
				//FPakFile::MakeDirectoryFromPath(Input.Dest);

				Input.bNeedsCompression = Entry.CompressionMethodIndex != 0;
				Input.bNeedEncryption = Entry.IsEncrypted();

				OutEntries->Add(Input);
			}
		}
		FMemory::Free(Buffer);
		FMemory::Free(PersistantCompressionBuffer);

		UE_LOG(LogTemp, Log, TEXT("Finished extracting %d (including %d errors)."), ExtractedCount, ErrorCount);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to open pak file \"%s\"."), InPakFilename);
		return false;
	}
	return true;
}

bool UAdvancePakLibrary::ExtractSingleFile(const TCHAR* InPakFilename, const TCHAR* InSingleFilename, TArray<uint8>& FileData, const FKeyChain& InKeyChain)
{
	FPakFile PakFile(&FPlatformFileManager::Get().GetPlatformFile(), InPakFilename, false);
	if (PakFile.IsValid())
	{
		FArchive& PakReader = *PakFile.GetSharedReader(NULL);
		const int64 BufferSize = 8 * 1024 * 1024; // 8MB buffer for extracting
		void* Buffer = FMemory::Malloc(BufferSize);
		int64 CompressionBufferSize = 0;
		uint8* PersistantCompressionBuffer = NULL;

		FPakEntry Entry;
		if (PakFile.Find(UAdvancePakLibrary::DefaultGameIniPath, &Entry) == FPakFile::EFindResult::Found)
		{

			PakReader.Seek(Entry.Offset);
			uint32 SerializedCrcTest = 0;
			FPakEntry EntryInfo;
			EntryInfo.Serialize(PakReader, PakFile.GetInfo().Version);
			if (EntryInfo == Entry)
			{
				FMemoryWriter MemoryFile(FileData);
				FArchive* FileHandle = &MemoryFile;

				if (Entry.CompressionMethodIndex == 0)
				{
					BufferedCopyFile(*FileHandle, PakReader, PakFile, Entry, Buffer, BufferSize, InKeyChain);
				}
				else
				{
					UncompressCopyFile(*FileHandle, PakReader, Entry, PersistantCompressionBuffer, CompressionBufferSize, InKeyChain, PakFile);
				}
			}
		}
		else
		{
			FMemory::Free(Buffer);
			FMemory::Free(PersistantCompressionBuffer);

			UE_LOG(LogTemp, Error, TEXT("Unable to open single file \"%s\" in pak \"%s\""), InSingleFilename, InPakFilename);

			return false;
		}

		FMemory::Free(Buffer);
		FMemory::Free(PersistantCompressionBuffer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to open pak file \"%s\"."), InPakFilename);
		return false;
	}
	return true;
}

void UAdvancePakLibrary::LoadKeyChainFromFile(const FString& InFilename, FKeyChain& OutCryptoSettings)
{
	FArchive* File = IFileManager::Get().CreateFileReader(*InFilename);
	UE_CLOG(File == nullptr, LogPakFile, Fatal, TEXT("Specified crypto keys cache '%s' does not exist!"), *InFilename);
	TSharedPtr<FJsonObject> RootObject;
	TSharedRef<TJsonReader<char>> Reader = TJsonReaderFactory<char>::Create(File);
	if (FJsonSerializer::Deserialize(Reader, RootObject))
	{
		const TSharedPtr<FJsonObject>* EncryptionKeyObject;
		if (RootObject->TryGetObjectField(TEXT("EncryptionKey"), EncryptionKeyObject))
		{
			FString EncryptionKeyBase64;
			if ((*EncryptionKeyObject)->TryGetStringField(TEXT("Key"), EncryptionKeyBase64))
			{
				if (EncryptionKeyBase64.Len() > 0)
				{
					TArray<uint8> Key;
					FBase64::Decode(EncryptionKeyBase64, Key);
					check(Key.Num() == sizeof(FAES::FAESKey::Key));
					FNamedAESKey NewKey;
					NewKey.Name = TEXT("Default");
					NewKey.Guid = FGuid();
					FMemory::Memcpy(NewKey.Key.Key, &Key[0], sizeof(FAES::FAESKey::Key));
					OutCryptoSettings.EncryptionKeys.Add(NewKey.Guid, NewKey);
				}
			}
		}

		const TSharedPtr<FJsonObject>* SigningKey = nullptr;
		if (RootObject->TryGetObjectField(TEXT("SigningKey"), SigningKey))
		{
			OutCryptoSettings.SigningKey = ParseRSAKeyFromJson(*SigningKey);
		}

		const TArray<TSharedPtr<FJsonValue>>* SecondaryEncryptionKeyArray = nullptr;
		if (RootObject->TryGetArrayField(TEXT("SecondaryEncryptionKeys"), SecondaryEncryptionKeyArray))
		{
			for (TSharedPtr<FJsonValue> EncryptionKeyValue : *SecondaryEncryptionKeyArray)
			{
				FNamedAESKey NewKey;
				TSharedPtr<FJsonObject> SecondaryEncryptionKeyObject = EncryptionKeyValue->AsObject();
				FGuid::Parse(SecondaryEncryptionKeyObject->GetStringField(TEXT("Guid")), NewKey.Guid);
				NewKey.Name = SecondaryEncryptionKeyObject->GetStringField(TEXT("Name"));
				FString KeyBase64 = SecondaryEncryptionKeyObject->GetStringField(TEXT("Key"));

				TArray<uint8> Key;
				FBase64::Decode(KeyBase64, Key);
				check(Key.Num() == sizeof(FAES::FAESKey::Key));
				FMemory::Memcpy(NewKey.Key.Key, &Key[0], sizeof(FAES::FAESKey::Key));

				check(!OutCryptoSettings.EncryptionKeys.Contains(NewKey.Guid) || OutCryptoSettings.EncryptionKeys[NewKey.Guid].Key == NewKey.Key);
				OutCryptoSettings.EncryptionKeys.Add(NewKey.Guid, NewKey);
			}
		}
	}
	delete File;
	FGuid EncryptionKeyOverrideGuid;
	OutCryptoSettings.MasterEncryptionKey = OutCryptoSettings.EncryptionKeys.Find(EncryptionKeyOverrideGuid);
}

void UAdvancePakLibrary::ApplyEncryptionKeys(const FKeyChain& KeyChain)
{
	if (KeyChain.EncryptionKeys.Contains(FGuid()))
	{
		FAES::FAESKey DefaultKey = KeyChain.EncryptionKeys[FGuid()].Key;
		FCoreDelegates::GetPakEncryptionKeyDelegate().BindLambda([DefaultKey](uint8 OutKey[32]) { FMemory::Memcpy(OutKey, DefaultKey.Key, sizeof(DefaultKey.Key)); });
	}

	for (const TMap<FGuid, FNamedAESKey>::ElementType& Key : KeyChain.EncryptionKeys)
	{
		if (Key.Key.IsValid())
		{
			FCoreDelegates::GetRegisterEncryptionKeyDelegate().ExecuteIfBound(Key.Key, Key.Value.Key);
		}
	}
}

FArchive* UAdvancePakLibrary::CreatePakWriter(const TCHAR* Filename, const FKeyChain& InKeyChain, bool bSign)
{
	FArchive* Writer = IFileManager::Get().CreateFileWriter(Filename);

	if (Writer)
	{
		if (bSign)
		{
			UE_LOG(LogTemp, Display, TEXT("Creating signed pak %s."), Filename);
			Writer = new FSignedArchiveWriter(*Writer, Filename, InKeyChain.SigningKey);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Creating pak %s."), Filename);
		}
	}

	return Writer;
}

bool UAdvancePakLibrary::ReadSizeParam(const TCHAR* CmdLine, const TCHAR* ParamStr, int64& SizeOut)
{
	FString ParamValueStr;
	if (FParse::Value(CmdLine, ParamStr, ParamValueStr) &&
		FParse::Value(CmdLine, ParamStr, SizeOut))
	{
		if (ParamValueStr.EndsWith(TEXT("GB")))
		{
			SizeOut *= 1024 * 1024 * 1024;
		}
		else if (ParamValueStr.EndsWith(TEXT("MB")))
		{
			SizeOut *= 1024 * 1024;
		}
		else if (ParamValueStr.EndsWith(TEXT("KB")))
		{
			SizeOut *= 1024;
		}
		return true;
	}
	return false;
}

void UAdvancePakLibrary::ProcessCommandLine(const TCHAR* CmdLine, FPakCommandLineParameters& CmdLineParameters)
{
	// List of all items to add to pak file
	FString ResponseFile;
	FString ClusterSizeString;

	if (FParse::Param(CmdLine, TEXT("patchcompatibilitymode421")))
	{
		CmdLineParameters.bPatchCompatibilityMode421 = true;
	}

	if (FParse::Param(CmdLine, TEXT("asynccompression")))
	{
		CmdLineParameters.bAsyncCompression = true;
	}

	if (FParse::Param(CmdLine, TEXT("fallbackOrderForNonUassetFiles")))
	{
		CmdLineParameters.bFallbackOrderForNonUassetFiles = true;
	}

	if (FParse::Value(CmdLine, TEXT("-blocksize="), ClusterSizeString) &&
		FParse::Value(CmdLine, TEXT("-blocksize="), CmdLineParameters.FileSystemBlockSize))
	{
		if (ClusterSizeString.EndsWith(TEXT("MB")))
		{
			CmdLineParameters.FileSystemBlockSize *= 1024 * 1024;
		}
		else if (ClusterSizeString.EndsWith(TEXT("KB")))
		{
			CmdLineParameters.FileSystemBlockSize *= 1024;
		}
	}
	else
	{
		CmdLineParameters.FileSystemBlockSize = 0;
	}

	FString CompBlockSizeString;
	if (FParse::Value(CmdLine, TEXT("-compressionblocksize="), CompBlockSizeString) &&
		FParse::Value(CmdLine, TEXT("-compressionblocksize="), CmdLineParameters.CompressionBlockSize))
	{
		if (CompBlockSizeString.EndsWith(TEXT("MB")))
		{
			CmdLineParameters.CompressionBlockSize *= 1024 * 1024;
		}
		else if (CompBlockSizeString.EndsWith(TEXT("KB")))
		{
			CmdLineParameters.CompressionBlockSize *= 1024;
		}
	}

	if (!FParse::Value(CmdLine, TEXT("-patchpaddingalign="), CmdLineParameters.PatchFilePadAlign))
	{
		CmdLineParameters.PatchFilePadAlign = 0;
	}

	if (!FParse::Value(CmdLine, TEXT("-AlignForMemoryMapping="), CmdLineParameters.AlignForMemoryMapping))
	{
		CmdLineParameters.AlignForMemoryMapping = 0;
	}

	if (FParse::Param(CmdLine, TEXT("encryptindex")))
	{
		CmdLineParameters.EncryptIndex = true;
	}

	if (FParse::Param(CmdLine, TEXT("sign")))
	{
		CmdLineParameters.bSign = true;
	}

	if (FParse::Param(CmdLine, TEXT("AlignFilesLargerThanBlock")))
	{
		CmdLineParameters.bAlignFilesLargerThanBlock = true;
	}

	FString DesiredCompressionFormats;
	// look for -compressionformats or -compressionformat on the commandline
	if (FParse::Value(CmdLine, TEXT("-compressionformats="), DesiredCompressionFormats) || FParse::Value(CmdLine, TEXT("-compressionformat="), DesiredCompressionFormats))
	{
		TArray<FString> Formats;
		DesiredCompressionFormats.ParseIntoArray(Formats, TEXT(","));
		for (FString& Format : Formats)
		{
			// look until we have a valid format
			FName FormatName = *Format;

			if (FCompression::IsFormatValid(FormatName))
			{
				CmdLineParameters.CompressionFormats.Add(FormatName);
				break;
			}
		}
	}

	// make sure we can always fallback to zlib, which is guaranteed to exist
	CmdLineParameters.CompressionFormats.AddUnique(NAME_Zlib);

	if (FParse::Value(CmdLine, TEXT("-create="), ResponseFile))
	{
		CmdLineParameters.GeneratePatch = FParse::Value(CmdLine, TEXT("-generatepatch="), CmdLineParameters.SourcePatchPakFilename);
		FParse::Value(CmdLine, TEXT("-outputchangedfiles="), CmdLineParameters.ChangedFilesOutputFilename);

		bool bCompress = FParse::Param(CmdLine, TEXT("compress"));
		bool bEncrypt = FParse::Param(CmdLine, TEXT("encrypt"));

		if (CmdLineParameters.GeneratePatch)
		{
			FParse::Value(CmdLine, TEXT("-patchSeekOptMaxInflationPercent="), CmdLineParameters.SeekOptParams.MaxInflationPercent);
			ReadSizeParam(CmdLine, TEXT("-patchSeekOptMaxGapSize="), CmdLineParameters.SeekOptParams.MaxGapSize);
			FParse::Value(CmdLine, TEXT("-patchSeekOptMaxAdjacentOrderDiff="), CmdLineParameters.SeekOptParams.MaxAdjacentOrderDiff);

			// For legacy reasons, if we specify a max gap size without a mode, we default to OnePass
			if (CmdLineParameters.SeekOptParams.MaxGapSize > 0)
			{
				CmdLineParameters.SeekOptParams.Mode = ESeekOptMode::OnePass;
			}
			FParse::Value(CmdLine, TEXT("-patchSeekOptMode="), (int32&)CmdLineParameters.SeekOptParams.Mode);
		}
	}
}

bool UAdvancePakLibrary::CreatePakFile(const TCHAR* Filename, TArray<FPakInputPair>& FilesToAdd, const FPakCommandLineParameters& CmdLineParameters, const FKeyChain& InKeyChain)
{
	const double StartTime = FPlatformTime::Seconds();

	// Create Pak
	TUniquePtr<FArchive> PakFileHandle(CreatePakWriter(Filename, InKeyChain, CmdLineParameters.bSign));
	if (!PakFileHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to create pak file \"%s\"."), Filename);
		return false;
	}

	FPakInfo Info;
	Info.bEncryptedIndex = (InKeyChain.MasterEncryptionKey && CmdLineParameters.EncryptIndex);
	Info.EncryptionKeyGuid = InKeyChain.MasterEncryptionKey ? InKeyChain.MasterEncryptionKey->Guid : FGuid();

	if (CmdLineParameters.bPatchCompatibilityMode421)
	{
		// for old versions, put in some known names that we may have used
		Info.GetCompressionMethodIndex(NAME_None);
		Info.GetCompressionMethodIndex(NAME_Zlib);
		Info.GetCompressionMethodIndex(NAME_Gzip);
		Info.GetCompressionMethodIndex(TEXT("Bogus"));
		Info.GetCompressionMethodIndex(TEXT("Oodle"));
	}

	TArray<FName> UsedCompressionFormats; // List of compression formats we actually used in this pak file (used for logging only)

	if (InKeyChain.MasterEncryptionKey)
	{
		UE_LOG(LogTemp, Display, TEXT("Using encryption key '%s' [%s]"), *InKeyChain.MasterEncryptionKey->Name, *InKeyChain.MasterEncryptionKey->Guid.ToString());
	}

	TArray<FPakEntryPair> Index;
	FString MountPoint = GetCommonRootPath(FilesToAdd);
	uint8* ReadBuffer = NULL;
	int64 BufferSize = 0;

	uint8* PaddingBuffer = nullptr;
	int64 PaddingBufferSize = 0;
	if (CmdLineParameters.PatchFilePadAlign > 0 || CmdLineParameters.AlignForMemoryMapping)
	{
		PaddingBufferSize = FMath::Max(CmdLineParameters.PatchFilePadAlign, CmdLineParameters.AlignForMemoryMapping);
		PaddingBuffer = (uint8*)FMemory::Malloc(PaddingBufferSize);
		FMemory::Memset(PaddingBuffer, 0, PaddingBufferSize);
	}

	// Some platforms provide patch download size reduction by diffing the patch files.  However, they often operate on specific block
	// sizes when dealing with new data within the file.  Pad files out to the given alignment to work with these systems more nicely.
	// We also want to combine smaller files into the same padding size block so we don't waste as much space. i.e. grouping 64 1k files together
	// rather than padding each out to 64k.
	const uint32 RequiredPatchPadding = CmdLineParameters.PatchFilePadAlign;

	uint64 ContiguousTotalSizeSmallerThanBlockSize = 0;
	uint64 ContiguousFilesSmallerThanBlockSize = 0;

	uint64 TotalUncompressedSize = 0;
	uint64 TotalCompressedSize = 0;

	uint64 TotalRequestedEncryptedFiles = 0;
	uint64 TotalEncryptedFiles = 0;
	uint64 TotalEncryptedDataSize = 0;

	TArray<FString> ExtensionsToNotUsePluginCompression;
	GConfig->GetArray(TEXT("Pak"), TEXT("ExtensionsToNotUsePluginCompression"), ExtensionsToNotUsePluginCompression, GEngineIni);
	TSet<FString> NoPluginCompressionExtensions;
	for (const FString& Ext : ExtensionsToNotUsePluginCompression)
	{
		NoPluginCompressionExtensions.Add(Ext);
	}

	struct FAsyncCompressor
	{

		// input
		const FPakInputPair* FileToAdd;
		FPakEntryPair Entry;
		const TArray<FName>* CompressionFormats;
		const TSet<FString>* NoPluginCompressionExtensions;

		// output
		FCompressedFileBuffer CompressedFileBuffer;
		FName CompressionMethod;
		int32 CompressionBlockSize;
		int64 RealFileSize;
		int64 OriginalFileSize;
		volatile bool bIsComplete;

		void Init(FPakInputPair* InFileToAdd, const TArray<FName>* InCompressionFormats, int32 InCompressionBlockSize, const TSet<FString>* InNoPluginCompressionExtensions)
		{
			CompressionMethod = NAME_None;
			if (InFileToAdd->bIsDeleteRecord)
			{
				// don't need to do anything for delete records
				bIsComplete = true;
				return;
			}
			FileToAdd = InFileToAdd;
			NoPluginCompressionExtensions = InNoPluginCompressionExtensions;
			CompressionFormats = InCompressionFormats;
			CompressionBlockSize = InCompressionBlockSize;
			bIsComplete = false;
		}

		bool IsComplete() const
		{
			return bIsComplete;
		}
		void Complete()
		{
			bIsComplete = true;
		}

		void Compress()
		{
			if (bIsComplete == true)
			{
				return;
			}

			//check if this file requested to be compression
			//OriginalFileSize = IFileManager::Get().FileSize(*FileToAdd->Source);
			OriginalFileSize = FileToAdd->Bytes.Num();
			RealFileSize = OriginalFileSize + Entry.Info.GetSerializedSize(FPakInfo::PakFile_Version_Latest);

			if (OriginalFileSize <= 0 || (FileToAdd->bNeedsCompression == false))
			{
				// done, don't need to do anything else
				Complete();
				return;
			}

			bool bSomeCompressionSucceeded = false;
			for (int32 MethodIndex = 0; MethodIndex < CompressionFormats->Num(); MethodIndex++)
			{
				CompressionMethod = (*CompressionFormats)[MethodIndex];

				// because compression is a plugin, certain files need to be loadable out of pak files before plugins are loadable
				// (like .uplugin files). for these, we enforce a non-plugin compression - zlib
				bool bForceCompressionFormat = false;
				if (NoPluginCompressionExtensions->Find(FPaths::GetExtension(FileToAdd->Source)) != nullptr)
				{
					CompressionMethod = NAME_Zlib;
					bForceCompressionFormat = true;
				}

				// attempt to compress the data
				if (CompressedFileBuffer.CompressFileToWorkingBuffer(*FileToAdd, CompressionMethod, CompressionBlockSize))
				{
					// Check the compression ratio, if it's too low just store uncompressed. Also take into account read size
					// if we still save 64KB it's probably worthwhile compressing, as that saves a file read operation in the runtime.
								// TODO: drive this threshold from the command line
					float PercentLess = ((float)CompressedFileBuffer.TotalCompressedSize / (OriginalFileSize / 100.f));
					if (PercentLess > 90.f && (OriginalFileSize - CompressedFileBuffer.TotalCompressedSize) < 65536)
					{
						// compression did not succeed, we can try the next format, so do nothing here
					}
					else
					{
						Entry.Info.CompressionBlocks.AddUninitialized(CompressedFileBuffer.CompressedBlocks.Num());
						RealFileSize = CompressedFileBuffer.TotalCompressedSize + Entry.Info.GetSerializedSize(FPakInfo::PakFile_Version_Latest);
						Entry.Info.CompressionBlocks.Reset();

						// at this point, we have successfully compressed the file, no need to continue
						bSomeCompressionSucceeded = true;
					}
				}

				// if we successfully compressed it, or we only wanted a single format, then we are done!
				if (bSomeCompressionSucceeded || bForceCompressionFormat)
				{
					break;
				}
			}

			// If no compression was able to make it small enough, or compress at all, don't compress it
			if (!bSomeCompressionSucceeded)
			{
				UE_LOG(LogTemp, Log, TEXT("File \"%s\" did not get small enough from compression, or compression failed."), *FileToAdd->Source);
				CompressionMethod = NAME_None;
			}
			Complete();
		}

		void CleanUp()
		{
			CompressedFileBuffer.Empty();
		}
	};



	FThreadSafeCounter CompletionCounter;

	TArray<FAsyncCompressor> AsyncCompressors;
	AsyncCompressors.AddZeroed(FilesToAdd.Num());

	for (int32 FileIndex = 0; FileIndex < FilesToAdd.Num(); FileIndex++)
	{
		AsyncCompressors[FileIndex].Init(&FilesToAdd[FileIndex], &CmdLineParameters.CompressionFormats, CmdLineParameters.CompressionBlockSize, &NoPluginCompressionExtensions);

		// call compress function inline 
		// it won't do anything except for initialize some internal variables used in the non compressed path
		// we don't want to pass these to a different thread as they may cause congestion with legitimate tasks
		AsyncCompressors[FileIndex].Compress();
	}

	for (int32 FileIndex = 0; FileIndex < FilesToAdd.Num(); FileIndex++)
	{
		bool bDeleted = FilesToAdd[FileIndex].bIsDeleteRecord;
		bool bIsUAssetUExpPairUAsset = false;
		bool bIsUAssetUExpPairUExp = false;
		bool bIsMappedBulk = FilesToAdd[FileIndex].Source.EndsWith(TEXT(".m.ubulk"));


		if (FileIndex)
		{
			if (FPaths::GetBaseFilename(FilesToAdd[FileIndex - 1].Dest, false) == FPaths::GetBaseFilename(FilesToAdd[FileIndex].Dest, false) &&
				FPaths::GetExtension(FilesToAdd[FileIndex - 1].Dest, true) == TEXT(".uasset") &&
				FPaths::GetExtension(FilesToAdd[FileIndex].Dest, true) == TEXT(".uexp")
				)
			{
				bIsUAssetUExpPairUExp = true;
			}
		}
		if (!bIsUAssetUExpPairUExp && FileIndex + 1 < FilesToAdd.Num())
		{
			if (FPaths::GetBaseFilename(FilesToAdd[FileIndex].Dest, false) == FPaths::GetBaseFilename(FilesToAdd[FileIndex + 1].Dest, false) &&
				FPaths::GetExtension(FilesToAdd[FileIndex].Dest, true) == TEXT(".uasset") &&
				FPaths::GetExtension(FilesToAdd[FileIndex + 1].Dest, true) == TEXT(".uexp")
				)
			{
				bIsUAssetUExpPairUAsset = true;
			}
		}

		//  Remember the offset but don't serialize it with the entry header.
		int64 NewEntryOffset = PakFileHandle->Tell();
		FPakEntryPair& NewEntry = AsyncCompressors[FileIndex].Entry;

		AsyncCompressors[FileIndex].Compress();

		const FName& CompressionMethod = AsyncCompressors[FileIndex].CompressionMethod;
		FCompressedFileBuffer& CompressedFileBuffer = AsyncCompressors[FileIndex].CompressedFileBuffer;

		if (!bDeleted)
		{
			const int64& RealFileSize = AsyncCompressors[FileIndex].RealFileSize;
			const int64& OriginalFileSize = AsyncCompressors[FileIndex].OriginalFileSize;
			const int32 CompressionBlockSize = AsyncCompressors[FileIndex].CompressionBlockSize;
			NewEntry.Info.CompressionMethodIndex = Info.GetCompressionMethodIndex(CompressionMethod);

			// Account for file system block size, which is a boundary we want to avoid crossing.
			if (!bIsUAssetUExpPairUExp && // don't split uexp / uasset pairs
				CmdLineParameters.FileSystemBlockSize > 0 &&
				OriginalFileSize != INDEX_NONE &&
				(CmdLineParameters.bAlignFilesLargerThanBlock || RealFileSize <= CmdLineParameters.FileSystemBlockSize) &&
				(NewEntryOffset / CmdLineParameters.FileSystemBlockSize) != ((NewEntryOffset + RealFileSize) / CmdLineParameters.FileSystemBlockSize)) // File crosses a block boundary
			{
				int64 OldOffset = NewEntryOffset;
				NewEntryOffset = AlignArbitrary(NewEntryOffset, CmdLineParameters.FileSystemBlockSize);
				int64 PaddingRequired = NewEntryOffset - OldOffset;

				if (PaddingRequired > 0)
				{
					// If we don't already have a padding buffer, create one
					if (PaddingBuffer == nullptr)
					{
						PaddingBufferSize = 64 * 1024;
						PaddingBuffer = (uint8*)FMemory::Malloc(PaddingBufferSize);
						FMemory::Memset(PaddingBuffer, 0, PaddingBufferSize);
					}

					UE_LOG(LogTemp, Verbose, TEXT("%14llu - %14llu : %14llu padding."), PakFileHandle->Tell(), PakFileHandle->Tell() + PaddingRequired, PaddingRequired);
					while (PaddingRequired > 0)
					{
						int64 AmountToWrite = FMath::Min(PaddingRequired, PaddingBufferSize);
						PakFileHandle->Serialize(PaddingBuffer, AmountToWrite);
						PaddingRequired -= AmountToWrite;
					}

					check(PakFileHandle->Tell() == NewEntryOffset);
				}
			}

			// Align bulk data
			if (bIsMappedBulk && CmdLineParameters.AlignForMemoryMapping > 0 && OriginalFileSize != INDEX_NONE && !bDeleted)
			{
				if (!IsAligned(NewEntryOffset + NewEntry.Info.GetSerializedSize(FPakInfo::PakFile_Version_Latest), CmdLineParameters.AlignForMemoryMapping))
				{
					int64 OldOffset = NewEntryOffset;
					NewEntryOffset = AlignArbitrary(NewEntryOffset + NewEntry.Info.GetSerializedSize(FPakInfo::PakFile_Version_Latest), CmdLineParameters.AlignForMemoryMapping) - NewEntry.Info.GetSerializedSize(FPakInfo::PakFile_Version_Latest);
					int64 PaddingRequired = NewEntryOffset - OldOffset;

					check(PaddingRequired > 0);
					check(PaddingBuffer && PaddingBufferSize >= PaddingRequired);

					{
						UE_LOG(LogTemp, Verbose, TEXT("%14llu - %14llu : %14llu bulk padding."), PakFileHandle->Tell(), PakFileHandle->Tell() + PaddingRequired, PaddingRequired);
						PakFileHandle->Serialize(PaddingBuffer, PaddingRequired);
						check(PakFileHandle->Tell() == NewEntryOffset);
					}
				}
			}
		}

		bool bCopiedToPak;
		int64 SizeToWrite = 0;
		uint8* DataToWrite = nullptr;
		if (bDeleted)
		{
			PrepareDeleteRecordForPak(MountPoint, FilesToAdd[FileIndex], NewEntry);
			bCopiedToPak = false;

			// Directly add the new entry to the index, no more work to do
			Index.Add(NewEntry);
		}
		else if (FilesToAdd[FileIndex].bNeedsCompression && CompressionMethod != NAME_None)
		{
			bCopiedToPak = PrepareCopyCompressedFileToPak(MountPoint, Info, FilesToAdd[FileIndex], CompressedFileBuffer, NewEntry, DataToWrite, SizeToWrite, InKeyChain);
			DataToWrite = CompressedFileBuffer.CompressedBuffer.Get();
		}
		else
		{
			bCopiedToPak = PrepareCopyFileToPak(MountPoint, FilesToAdd[FileIndex], ReadBuffer, BufferSize, NewEntry, DataToWrite, SizeToWrite, InKeyChain);
			DataToWrite = ReadBuffer;
		}

		int64 TotalSizeToWrite = SizeToWrite + NewEntry.Info.GetSerializedSize(FPakInfo::PakFile_Version_Latest);
		if (bCopiedToPak)
		{
			if (RequiredPatchPadding > 0 &&
				!(bIsMappedBulk && CmdLineParameters.AlignForMemoryMapping > 0) // don't wreck the bulk padding with patch padding
				)
			{
				//if the next file is going to cross a patch-block boundary then pad out the current set of files with 0's
				//and align the next file up.
				bool bCrossesBoundary = AlignArbitrary(NewEntryOffset, RequiredPatchPadding) != AlignArbitrary(NewEntryOffset + TotalSizeToWrite - 1, RequiredPatchPadding);
				bool bPatchPadded = false;
				if (!bIsUAssetUExpPairUExp) // never patch-pad the uexp of a uasset/uexp pair
				{
					bool bPairProbablyCrossesBoundary = false; // we don't consider compression because we have not compressed the uexp yet.
					if (bIsUAssetUExpPairUAsset)
					{
						//int64 UExpFileSize = IFileManager::Get().FileSize(*FilesToAdd[FileIndex + 1].Source) / 2; // assume 50% compression
						int64 UExpFileSize = FilesToAdd[FileIndex + 1].Bytes.Num() / 2;
						bPairProbablyCrossesBoundary = AlignArbitrary(NewEntryOffset, RequiredPatchPadding) != AlignArbitrary(NewEntryOffset + TotalSizeToWrite + UExpFileSize - 1, RequiredPatchPadding);
					}
					if (TotalSizeToWrite >= RequiredPatchPadding || // if it exactly the padding size and by luck does not cross a boundary, we still consider it "over" because it can't be packed with anything else
						bCrossesBoundary || bPairProbablyCrossesBoundary)
					{
						NewEntryOffset = AlignArbitrary(NewEntryOffset, RequiredPatchPadding);
						int64 CurrentLoc = PakFileHandle->Tell();
						int64 PaddingSize = NewEntryOffset - CurrentLoc;
						check(PaddingSize >= 0);
						if (PaddingSize)
						{
							UE_LOG(LogTemp, Verbose, TEXT("%14llu - %14llu : %14llu patch padding."), PakFileHandle->Tell(), PakFileHandle->Tell() + PaddingSize, PaddingSize);
							check(PaddingSize <= PaddingBufferSize);

							//have to pad manually with 0's.  File locations skipped by Seek and never written are uninitialized which would defeat the whole purpose
							//of padding for certain platforms patch diffing systems.
							PakFileHandle->Serialize(PaddingBuffer, PaddingSize);
						}
						check(PakFileHandle->Tell() == NewEntryOffset);
						bPatchPadded = true;
					}
				}

				//if the current file is bigger than a patch block then we will always have to pad out the previous files.
				//if there were a large set of contiguous small files behind us then this will be the natural stopping point for a possible pathalogical patching case where growth in the small files causes a cascade 
				//to dirty up all the blocks prior to this one.  If this could happen let's warn about it.
				if (bPatchPadded ||
					FileIndex + 1 == FilesToAdd.Num()) // also check the last file, this won't work perfectly if we don't end up adding the last file for some reason
				{
					const uint64 ContiguousGroupedFilePatchWarningThreshhold = 50 * 1024 * 1024;
					if (ContiguousTotalSizeSmallerThanBlockSize > ContiguousGroupedFilePatchWarningThreshhold)
					{
						UE_LOG(LogTemp, Display, TEXT("%i small files (%i) totaling %llu contiguous bytes found before first 'large' file.  Changes to any of these files could cause the whole group to be 'dirty' in a per-file binary diff based patching system."), ContiguousFilesSmallerThanBlockSize, RequiredPatchPadding, ContiguousTotalSizeSmallerThanBlockSize);
					}
					ContiguousTotalSizeSmallerThanBlockSize = 0;
					ContiguousFilesSmallerThanBlockSize = 0;
				}
				else
				{
					ContiguousTotalSizeSmallerThanBlockSize += TotalSizeToWrite;
					ContiguousFilesSmallerThanBlockSize++;
				}
			}
			if (FilesToAdd[FileIndex].bNeedsCompression && CompressionMethod != NAME_None)
			{
				UsedCompressionFormats.AddUnique(CompressionMethod); // used for logging only
				FinalizeCopyCompressedFileToPak(Info, CompressedFileBuffer, NewEntry);
			}

			// Write to file
			int64 Offset = PakFileHandle->Tell();
			NewEntry.Info.Serialize(*PakFileHandle, FPakInfo::PakFile_Version_Latest);
			int64 PayloadOffset = PakFileHandle->Tell();
			PakFileHandle->Serialize(DataToWrite, SizeToWrite);
			int64 EndOffset = PakFileHandle->Tell();

			UE_LOG(LogTemp, Verbose, TEXT("%14llu [header] - %14llu - %14llu : %14llu header+file %s."), Offset, PayloadOffset, EndOffset, EndOffset - Offset, *NewEntry.Filename);

			// Update offset now and store it in the index (and only in index)
			NewEntry.Info.Offset = NewEntryOffset;
			Index.Add(NewEntry);
			const TCHAR* EncryptedString = TEXT("");

			if (FilesToAdd[FileIndex].bNeedEncryption)
			{
				TotalRequestedEncryptedFiles++;

				if (InKeyChain.MasterEncryptionKey)
				{
					TotalEncryptedFiles++;
					TotalEncryptedDataSize += SizeToWrite;
					EncryptedString = TEXT("encrypted ");
				}
			}

			if (FilesToAdd[FileIndex].bNeedsCompression && CompressionMethod != NAME_None)
			{
				TotalCompressedSize += NewEntry.Info.Size;
				TotalUncompressedSize += NewEntry.Info.UncompressedSize;
				float PercentLess = ((float)NewEntry.Info.Size / (NewEntry.Info.UncompressedSize / 100.f));
				if (FilesToAdd[FileIndex].SuggestedOrder < MAX_uint64)
				{
					UE_LOG(LogTemp, Log, TEXT("Added compressed %sfile \"%s\", %.2f%% of original size. Compressed with %s, Size %lld bytes, Original Size %lld bytes (order %llu)."), EncryptedString, *NewEntry.Filename, PercentLess, *CompressionMethod.ToString(), NewEntry.Info.Size, NewEntry.Info.UncompressedSize, FilesToAdd[FileIndex].SuggestedOrder);
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Added compressed %sfile \"%s\", %.2f%% of original size. Compressed with %s, Size %lld bytes, Original Size %lld bytes (no order given)."), EncryptedString, *NewEntry.Filename, PercentLess, *CompressionMethod.ToString(), NewEntry.Info.Size, NewEntry.Info.UncompressedSize);
				}
			}
			else
			{
				if (FilesToAdd[FileIndex].SuggestedOrder < MAX_uint64)
				{
					UE_LOG(LogTemp, Log, TEXT("Added %sfile \"%s\", %lld bytes (order %llu)."), EncryptedString, *NewEntry.Filename, NewEntry.Info.Size, FilesToAdd[FileIndex].SuggestedOrder);
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Added %sfile \"%s\", %lld bytes (no order given)."), EncryptedString, *NewEntry.Filename, NewEntry.Info.Size);
				}
			}
		}
		else
		{
			if (bDeleted)
			{
				UE_LOG(LogTemp, Log, TEXT("Created delete record for file \"%s\"."), *FilesToAdd[FileIndex].Source);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Missing file \"%s\" will not be added to PAK file."), *FilesToAdd[FileIndex].Source);
			}
		}
		AsyncCompressors[FileIndex].CleanUp();
	}

	FMemory::Free(PaddingBuffer);
	FMemory::Free(ReadBuffer);
	ReadBuffer = NULL;


	// Remember IndexOffset
	Info.IndexOffset = PakFileHandle->Tell();

	// Serialize Pak Index at the end of Pak File
	TArray<uint8> IndexData;
	FMemoryWriter IndexWriter(IndexData);
	IndexWriter.SetByteSwapping(PakFileHandle->ForceByteSwapping());
	int32 NumEntries = Index.Num();
	IndexWriter << MountPoint;
	IndexWriter << NumEntries;
	for (int32 EntryIndex = 0; EntryIndex < Index.Num(); EntryIndex++)
	{
		FPakEntryPair& Entry = Index[EntryIndex];
		IndexWriter << Entry.Filename;
		Entry.Info.Serialize(IndexWriter, Info.Version);

		if (RequiredPatchPadding > 0)
		{
			int64 EntrySize = Entry.Info.GetSerializedSize(FPakInfo::PakFile_Version_Latest);
			int64 TotalSizeToWrite = Entry.Info.Size + EntrySize;
			if (TotalSizeToWrite >= RequiredPatchPadding)
			{
				int64 RealStart = Entry.Info.Offset;
				if ((RealStart % RequiredPatchPadding) != 0 &&
					!Entry.Filename.EndsWith(TEXT("uexp")) && // these are export sections of larger files and may be packed with uasset/umap and so we don't need a warning here
					!(Entry.Filename.EndsWith(TEXT(".m.ubulk")) && CmdLineParameters.AlignForMemoryMapping > 0)) // Bulk padding unaligns patch padding and so we don't need a warning here
				{
					UE_LOG(LogTemp, Warning, TEXT("File at offset %lld of size %lld not aligned to patch size %i"), RealStart, Entry.Info.Size, RequiredPatchPadding);
				}
			}
		}
	}

	if (Info.bEncryptedIndex)
	{
		int32 OriginalSize = IndexData.Num();
		int32 AlignedSize = Align(OriginalSize, FAES::AESBlockSize);

		for (int32 PaddingIndex = IndexData.Num(); PaddingIndex < AlignedSize; ++PaddingIndex)
		{
			uint8 Byte = IndexData[PaddingIndex % OriginalSize];
			IndexData.Add(Byte);
		}
	}

	FSHA1::HashBuffer(IndexData.GetData(), IndexData.Num(), Info.IndexHash.Hash);

	if (Info.bEncryptedIndex)
	{
		check(InKeyChain.MasterEncryptionKey);
		FAES::EncryptData(IndexData.GetData(), IndexData.Num(), InKeyChain.MasterEncryptionKey->Key);
		TotalEncryptedDataSize += IndexData.Num();
	}

	PakFileHandle->Serialize(IndexData.GetData(), IndexData.Num());

	Info.IndexSize = IndexData.Num();

	// Save trailer (offset, size, hash value)
	Info.Serialize(*PakFileHandle, FPakInfo::PakFile_Version_Latest);

	UE_LOG(LogTemp, Display, TEXT("Added %d files, %lld bytes total, time %.2lfs."), Index.Num(), PakFileHandle->TotalSize(), FPlatformTime::Seconds() - StartTime);
	if (TotalUncompressedSize)
	{
		float PercentLess = ((float)TotalCompressedSize / (TotalUncompressedSize / 100.f));
		UE_LOG(LogTemp, Display, TEXT("Compression summary: %.2f%% of original size. Compressed Size %lld bytes, Original Size %lld bytes. "), PercentLess, TotalCompressedSize, TotalUncompressedSize);

		FString UsedCompressionFormatsString;
		for (FName CompressionFormat : UsedCompressionFormats)
		{
			UsedCompressionFormatsString.Append(CompressionFormat.ToString() + TEXT(", "));
		}

		UE_LOG(LogTemp, Display, TEXT("Used compression formats (in priority order) '%s'"), *UsedCompressionFormatsString);
	}

	if (TotalEncryptedDataSize)
	{
		UE_LOG(LogTemp, Display, TEXT("Encryption - ENABLED"));
		UE_LOG(LogTemp, Display, TEXT("  Files: %d"), TotalEncryptedFiles);

		if (Info.bEncryptedIndex)
		{
			UE_LOG(LogTemp, Display, TEXT("  Index: Encrypted (%d bytes, %.2fMB)"), Info.IndexSize, (float)Info.IndexSize / 1024.0f / 1024.0f);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("  Index: Unencrypted"));
		}


		UE_LOG(LogTemp, Display, TEXT("  Total: %d bytes (%.2fMB)"), TotalEncryptedDataSize, (float)TotalEncryptedDataSize / 1024.0f / 1024.0f);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Encryption - DISABLED"));
	}

	if (TotalEncryptedFiles < TotalRequestedEncryptedFiles)
	{
		UE_LOG(LogTemp, Display, TEXT("%d files requested encryption, but no AES key was supplied! Encryption was skipped for these files"), TotalRequestedEncryptedFiles);
	}

	PakFileHandle->Close();
	PakFileHandle.Reset();

	return true;
}

FString UAdvancePakLibrary::SigSuffixName(TEXT(".sig"));

float UAdvancePakLibrary::GetDiskFreeSpaceMB()
{
	uint64 TotalDiskSpace = 0;
	uint64 TotalDiskFreeSpace = 0;
	if (FPlatformMisc::GetDiskTotalAndFreeSpace(FPaths::ProjectDir(), TotalDiskSpace, TotalDiskFreeSpace))
	{
		double DiskFreeSpace = (double)TotalDiskFreeSpace;
		double DiskFreeSpaceMB = FUnitConversion::Convert(DiskFreeSpace, EUnit::Bytes, EUnit::Megabytes);
		return (float)DiskFreeSpaceMB;
	}
	return 0.0f;
}

FString UAdvancePakLibrary::CmdLineName(TEXT("CmdLine"));

uint64 UAdvancePakLibrary::GetDiskFreeSpace()
{
	uint64 TotalDiskSpace = 0;
	uint64 TotalDiskFreeSpace = 0;
	if (FPlatformMisc::GetDiskTotalAndFreeSpace(FPaths::ProjectDir(), TotalDiskSpace, TotalDiskFreeSpace))
	{
		return TotalDiskFreeSpace;
	}
	return 0;
}

FString UAdvancePakLibrary::DeleteResourcesName(TEXT("DeleteResources"));

FString UAdvancePakLibrary::PatchNameName(TEXT("PatchName"));

FString UAdvancePakLibrary::DefaultCryptoPath(TEXT(""));

FString UAdvancePakLibrary::DeletePatchsName(TEXT("DeletePatchs"));

FString UAdvancePakLibrary::AbsorbVersionConfigToJson(const TSharedPtr<FAdvancePakVersionConfig>& VersionConfig)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField(CmdLineName, VersionConfig->CmdLine);

	TArray<TSharedPtr<FJsonValue>> AddPatchsArray;
	for (auto PatchName : VersionConfig->AddPatchs)
	{
		AddPatchsArray.Add(MakeShared<FJsonValueString>(PatchName));
	}
	JsonObject->SetArrayField(AddPatchsName, AddPatchsArray);

	TArray<TSharedPtr<FJsonValue>> KeepPatchsArray;
	for (auto PatchName : VersionConfig->KeepPatchs)
	{
		KeepPatchsArray.Add(MakeShared<FJsonValueString>(PatchName));
	}
	JsonObject->SetArrayField(KeepPatchsName, KeepPatchsArray);

	TArray<TSharedPtr<FJsonValue>> DeletePatchsArray;
	for (auto PatchName : VersionConfig->DeletePatchs)
	{
		DeletePatchsArray.Add(MakeShared<FJsonValueString>(PatchName));
	}
	JsonObject->SetArrayField(DeletePatchsName, DeletePatchsArray);

	TArray<TSharedPtr<FJsonValue>> UpdatePatchsArray;
	for (auto UpdatePatch : VersionConfig->UpdatePatchs)
	{
		TSharedPtr<FJsonObject> PatchInfoObject = MakeShareable(new FJsonObject);

		PatchInfoObject->SetStringField(PatchNameName, UpdatePatch.Key);

		TArray<TSharedPtr<FJsonValue>> AddResourcesArray;
		for (auto Resource : UpdatePatch.Value->AddResources)
		{
			AddResourcesArray.Add(MakeShared<FJsonValueString>(Resource));
		}
		PatchInfoObject->SetArrayField(AddResourcesName, AddResourcesArray);

		TArray<TSharedPtr<FJsonValue>> UpdateResourcesArray;
		for (auto Resource : UpdatePatch.Value->UpdateResources)
		{
			UpdateResourcesArray.Add(MakeShared<FJsonValueString>(Resource));
		}
		PatchInfoObject->SetArrayField(UpdateResourcesName, UpdateResourcesArray);

		TArray<TSharedPtr<FJsonValue>> DeleteResourcesArray;
		for (auto Resource : UpdatePatch.Value->DeleteResources)
		{
			DeleteResourcesArray.Add(MakeShared<FJsonValueString>(Resource));
		}
		PatchInfoObject->SetArrayField(DeleteResourcesName, DeleteResourcesArray);

		TSharedPtr<FJsonValueObject> PatchInfoValue = MakeShareable(new FJsonValueObject(PatchInfoObject));
		UpdatePatchsArray.Add(PatchInfoValue);
	}
	JsonObject->SetArrayField(UpdatePatchsName, UpdatePatchsArray);

	TArray<TSharedPtr<FJsonValue>> PatchPropsArray;
	for (auto PatchProp : VersionConfig->PatchProps)
	{
		TSharedPtr<FJsonObject> PatchPropObject = MakeShareable(new FJsonObject);

		PatchPropObject->SetStringField(PatchNameName, PatchProp.Key);
		PatchPropObject->SetStringField(SizeName, FString::Printf(TEXT("%llu"), PatchProp.Value->Size));
		PatchPropObject->SetStringField(HashName, PatchProp.Value->Hash);

		TSharedPtr<FJsonValueObject> HashPatchValue = MakeShareable(new FJsonValueObject(PatchPropObject));
		PatchPropsArray.Add(HashPatchValue);
	}
	JsonObject->SetArrayField(PatchPropsName, PatchPropsArray);

	FString JsonContext;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonContext);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	return JsonContext;
}

FString UAdvancePakLibrary::AddPatchsName(TEXT("AddPatchs"));

void UAdvancePakLibrary::InitializeLibrary()
{
	DefaultGameIniPath = FPaths::Combine(FPaths::ProjectConfigDir(), FString("DefaultGame.ini"));
	DefaultCryptoPath = FPaths::Combine(FPaths::ProjectConfigDir(), FString("DefaultCrypto.ini"));

	ContentPaksPath = FString::Printf(TEXT("%sPaks"), *FPaths::ProjectContentDir());
	SavedPaksPath = FString::Printf(TEXT("%sPaks"), *FPaths::ProjectSavedDir());
	BufferPaksPath = FString::Printf(TEXT("%sBuffer"), *FPaths::ProjectSavedDir());
	StorePaksPath = FString::Printf(TEXT("%sStore"), *FPaths::ProjectSavedDir());
	MergePaksPath = FString::Printf(TEXT("%sMerge"), *FPaths::ProjectSavedDir());

	if (!IFileManager::Get().DirectoryExists(*SavedPaksPath))
		IFileManager::Get().MakeDirectory(*SavedPaksPath);

	if (!IFileManager::Get().DirectoryExists(*BufferPaksPath))
		IFileManager::Get().MakeDirectory(*BufferPaksPath);

	if (!IFileManager::Get().DirectoryExists(*StorePaksPath))
		IFileManager::Get().MakeDirectory(*StorePaksPath);

	if (!IFileManager::Get().DirectoryExists(*MergePaksPath))
		IFileManager::Get().MakeDirectory(*MergePaksPath);
}

FString UAdvancePakLibrary::BufferPaksPath(TEXT(""));

FString UAdvancePakLibrary::StorePaksPath(TEXT(""));

FString UAdvancePakLibrary::MergePaksPath(TEXT(""));

FString UAdvancePakLibrary::PatchPropsName(TEXT("PatchProps"));

FString UAdvancePakLibrary::KeepPatchsName(TEXT("KeepPatchs"));

TSharedPtr<FAdvancePakVersionConfig> UAdvancePakLibrary::AbsorbJsonToVersionConfig(const FString& JsonContext)
{
	TSharedPtr<FAdvancePakVersionConfig> VersionConfig;
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonContext);
	if (FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		VersionConfig = MakeShareable(new FAdvancePakVersionConfig());

		VersionConfig->CmdLine = JsonValue->AsObject()->GetStringField(CmdLineName);

		for (auto AddPatch : JsonValue->AsObject()->GetArrayField(AddPatchsName))
		{
			VersionConfig->AddPatchs.Add(AddPatch->AsString());
		}

		for (auto KeepPatch : JsonValue->AsObject()->GetArrayField(KeepPatchsName))
		{
			VersionConfig->KeepPatchs.Add(KeepPatch->AsString());
		}

		for (auto DeletePatch : JsonValue->AsObject()->GetArrayField(DeletePatchsName))
		{
			VersionConfig->DeletePatchs.Add(DeletePatch->AsString());
		}

		for (auto UpdatePatch : JsonValue->AsObject()->GetArrayField(UpdatePatchsName))
		{
			TSharedPtr<FAdvancePakPatchInfo> PatchInfo = MakeShareable(new FAdvancePakPatchInfo());

			for (auto AddResource : UpdatePatch->AsObject()->GetArrayField(AddResourcesName))
			{
				PatchInfo->AddResources.Add(AddResource->AsString());
			}

			for (auto UpdateResource : UpdatePatch->AsObject()->GetArrayField(UpdateResourcesName))
			{
				PatchInfo->UpdateResources.Add(UpdateResource->AsString());
			}

			for (auto DeleteResource : UpdatePatch->AsObject()->GetArrayField(DeleteResourcesName))
			{
				PatchInfo->DeleteResources.Add(DeleteResource->AsString());
			}

			VersionConfig->UpdatePatchs.Add(UpdatePatch->AsObject()->GetStringField(PatchNameName), PatchInfo);
		}

		for (auto PatchProp : JsonValue->AsObject()->GetArrayField(PatchPropsName))
		{
			TSharedPtr<FAdvancePakPatchProp> NewPatchProp = MakeShareable(new FAdvancePakPatchProp());
			NewPatchProp->Size = FCString::Strtoui64(*PatchProp->AsObject()->GetStringField(SizeName), nullptr, 10);
			NewPatchProp->Hash = PatchProp->AsObject()->GetStringField(HashName);
			VersionConfig->PatchProps.Add(PatchProp->AsObject()->GetStringField(PatchNameName), NewPatchProp);
		}
	}
	return VersionConfig;
}

FString UAdvancePakLibrary::ContentPaksPath(TEXT(""));

FString UAdvancePakLibrary::PublishName(TEXT("Publish"));

FString UAdvancePakLibrary::PakSuffixName(TEXT(".pak"));

FString UAdvancePakLibrary::SavedPaksPath(TEXT(""));

FString UAdvancePakLibrary::HashName(TEXT("Hash"));

FString UAdvancePakLibrary::SizeName(TEXT("Size"));

FString UAdvancePakLibrary::AddResourcesName(TEXT("AddResources"));

FString UAdvancePakLibrary::AdvancePakConfigName(TEXT("AdvancePakConfig"));

FString UAdvancePakLibrary::AdvancePakStateName(TEXT("AdvancePakState"));

FString UAdvancePakLibrary::VersionConfigName(TEXT("VersionConfig.json"));

FString UAdvancePakLibrary::UpdatePatchsName(TEXT("UpdatePatchs"));

FString UAdvancePakLibrary::ReducePatchsName(TEXT("ReducePatchs"));

FString UAdvancePakLibrary::GetVersionURL(const FString& JsonContext, const FString& Platform)
{
	TMap<FString, FString> PlatformInfos;
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonContext);
	if (FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		for (auto PlatformInfo : JsonValue->AsObject()->Values)
		{
			if (PlatformInfo.Key.Equals(Platform))
				return PlatformInfo.Value->AsString();
		}
	}
	return FString();
}

FString UAdvancePakLibrary::DefaultGameIniPath(TEXT(""));

FString UAdvancePakLibrary::VersionRootURL(TEXT(""));

FString UAdvancePakLibrary::UpdateResourcesName(TEXT("UpdateResources"));


