#include "AFileSystem.h"

#include "ADebug.h"

FileSystem::Save FileSystem::save;
#ifdef AUTO_PAIR
FileSystem::Config FileSystem::config;
#endif

FileData FileSystem::saveData(&LittleFS, "/save.data", 'B', &FileSystem::save, sizeof(FileSystem::Save));
#ifdef AUTO_PAIR
FileData FileSystem::configData(&LittleFS, "/config.data", 'B', &FileSystem::config, sizeof(FileSystem::Config));
#endif

#ifdef AUTO_PAIR
void FileSystem::Config::SetBLEAddr(const esp_bd_addr_t addr) noexcept
{
  for(size_t i = 0; i < 6; ++i)
    arrBLEAddress[i] = addr[i];
}

void FileSystem::Config::GetBLEAddr(esp_bd_addr_t& addr) noexcept
{
  for(size_t i = 0; i < 6; ++i)
    addr[i] = arrBLEAddress[i];
}

bool FileSystem::ReadConfig() noexcept
{
  FDstat_t stat = configData.read();

  switch (stat)
  {
    case FD_WRITE:
    case FD_ADD:
    case FD_READ:
      return true;
    case FD_FS_ERR:
    case FD_FILE_ERR:
    default:
      #ifdef DEBUG
      Serial.println("read config error!");
      #endif
      return false;
  }
}

bool FileSystem::WriteConfig() noexcept
{
  FDstat_t stat = configData.write();

  switch (stat)
  {
    case FD_WRITE:
      return true;
    case FD_FS_ERR:
    case FD_FILE_ERR:
    default:
      #ifdef DEBUG
      Serial.println("write config error!");
      #endif
      return false;
  }
}
#endif

bool FileSystem::WriteSettings() noexcept
{
  FDstat_t stat = saveData.write();

  switch (stat)
  {
    case FD_WRITE:
      return true;
    case FD_FS_ERR:
    case FD_FILE_ERR:
    default:
      #ifdef DEBUG
      Serial.println("write save error!");
      #endif
      return false;
  }
}
bool FileSystem::ReadSettings() noexcept
{
  FDstat_t stat = saveData.read();

  switch (stat)
  {
    case FD_WRITE:
    case FD_ADD:
    case FD_READ:
      return true;
    case FD_FS_ERR:
    case FD_FILE_ERR:
    default:
      #ifdef DEBUG
      Serial.println("read save error!");
      #endif
      return false;
  }
}

bool FileSystem::SetupSaveStartUp() noexcept
{
  LittleFS.begin(true);

  return ReadSettings();
}

#ifdef AUTO_PAIR
bool FileSystem::SetupConfigStartUp() noexcept
{
  return ReadConfig();
}
#endif