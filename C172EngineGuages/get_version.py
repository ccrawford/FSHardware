Import("env")
import os

# Get the version number from the build environment.
firmware_version = os.environ.get('VERSION', "")

# Clean up the version number
if firmware_version == "":
  # When no version is specified default to "0.0.1" for
  # compatibility with MobiFlight desktop app version checks.
  firmware_version = "0.0.1"

# Strip any leading "v" that might be on the version and
# any leading or trailing periods.
firmware_version = firmware_version.lstrip("v")
firmware_version = firmware_version.strip(".")

# Get the core version number from the custom.ini file
try: 
  core_firmware_version = env.GetProjectOption("custom_core_firmware_version")
  core_firmware_version = "2.5.1"   # Delete this once all PR's from the core FW repo are merged!!
  if core_firmware_version == "":
    # When no version is specified send an error
    print("ERROR!! Core Firmware Version must be a valid version!!")
    exit -1
except:
  print("ERROR!! Core Firmware Version must be defined!!")
  exit -1

print(f'Using version {firmware_version} for the build')
print(f'Using version {core_firmware_version} as core version')

# Append the version to the build defines so it gets baked into the firmware
env.Append(CPPDEFINES=[
  f'BUILD_VERSION={firmware_version}'
])
env.Append(CPPDEFINES=[
  f'CORE_BUILD_VERSION={core_firmware_version}'
])

# Set the output filename to the name of the board and the version
env.Replace(PROGNAME=f'{env["PIOENV"]}_{firmware_version.replace(".", "_")}')
