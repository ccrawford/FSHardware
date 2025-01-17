Import("env")
import os, zipfile, shutil
from pathlib import Path

# Get the version number from the build environment.
firmware_version = os.environ.get('VERSION', "")
if firmware_version == "":
    firmware_version = "0.0.1"
firmware_version = firmware_version.lstrip("v")
firmware_version = firmware_version.strip(".")

# Get the ZIP filename from the build environment.
community_project = env.GetProjectOption('custom_community_project', "")

# Get the custom folder from the build environment.
custom_source_folder = env.GetProjectOption('custom_source_folder', "")

# Get the foldername inside the zip file from the build environment.
community_folder = env.GetProjectOption('custom_community_folder', "")

platform = env.BoardConfig().get("platform", {})

def copy_fw_files (source, target, env):
    fw_file_name=str(target[0])

    if os.path.exists("./_build/" + custom_source_folder) == False:
        os.makedirs("./_build/" + custom_source_folder + "/Community/firmware")
        shutil.copytree(custom_source_folder + "/Community", "./_build/" + custom_source_folder + "/Community", dirs_exist_ok=True)
        print("Creating /_build folder")
    
    if platform == "raspberrypi":
        fw_file_name=fw_file_name[0:-3] + "uf2"

    if platform == "espressif32":
        merge_bin(source, target, env)
        old_name = fw_file_name[0:-4] + "_merged.bin"
        fw_file_name = fw_file_name[0:-9]  + "merged_" + firmware_version.replace(".", "_") + ".bin"
        os.replace(old_name, fw_file_name)

    print("Copying community folder")
    shutil.copy(fw_file_name, "./_build/" + custom_source_folder + "/Community/firmware")
    original_folder_path = "./_build/" + custom_source_folder + "/Community"
    zip_file_path = './_dist/' + community_project + '_' + firmware_version + '.zip'
    new_folder_in_zip = community_folder
    print("Creating zip file")
    createZIP(original_folder_path, zip_file_path, new_folder_in_zip)

def createZIP(original_folder_path, zip_file_path, new_folder_name):
    if os.path.exists("./_dist") == False:
        os.mkdir("./_dist")
    with zipfile.ZipFile(zip_file_path, 'w') as zipf:
        for root, dirs, files in os.walk(original_folder_path):
            for file in files:
                # Create a new path in the ZIP file
                new_path = os.path.join(new_folder_name, os.path.relpath(os.path.join(root, file), original_folder_path))
                # Add the file to the ZIP file
                zipf.write(os.path.join(root, file), new_path)

APP_BIN = "$BUILD_DIR/${PROGNAME}.bin"
MERGED_BIN = "$BUILD_DIR/${PROGNAME}_merged.bin"
BOARD_CONFIG = env.BoardConfig()

def merge_bin(source, target, env):
    # The list contains all extra images (bootloader, partitions, eboot) and
    # the final application binary
    flash_images = env.Flatten(env.get("FLASH_EXTRA_IMAGES", [])) + ["$ESP32_APP_OFFSET", APP_BIN]

    # Run esptool to merge images into a single binary
    env.Execute(
        " ".join(
            [
                "$PYTHONEXE",
                "$OBJCOPY",
                "--chip",
                BOARD_CONFIG.get("build.mcu", "esp32"),
                "merge_bin",
                "--fill-flash-size",
                BOARD_CONFIG.get("upload.flash_size", "4MB"),
                "-o",
                MERGED_BIN,
            ]
            + flash_images
        )
    )

# Patch the upload command to flash the merged binary at address 0x0
#env.Replace(
#    UPLOADERFLAGS=[
#        ]
#        + ["write_flash", "0x0", MERGED_BIN],
#    UPLOADCMD='"$PYTHONEXE" "$UPLOADER" $UPLOADERFLAGS',
#)

env.AddPostAction("$BUILD_DIR/${PROGNAME}.hex", copy_fw_files)
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", copy_fw_files)