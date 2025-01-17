import sys
import os
from PIL import Image

def convert_to_565(r, g, b):
    """Convert 8-bit RGB values to 16-bit 565 format."""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def write_header_file(filename, width, height, img_data):
    """Write the converted image data to a .h header file."""
    base_name = os.path.splitext(os.path.basename(filename))[0].upper()
    header_filename = f"{base_name.lower()}.h"

    with open(header_filename, "w") as header_file:
        header_file.write(f"#ifndef {base_name}_H\n")
        header_file.write(f"#define {base_name}_H\n\n")
        header_file.write(f"#define {base_name}_WIDTH {width}\n")
        header_file.write(f"#define {base_name}_HEIGHT {height}\n\n")
        header_file.write(f"const unsigned short {base_name}_IMG_DATA[{len(img_data)}] PROGMEM = {{\n")

        for i, value in enumerate(img_data):
            if i % 10 == 0:
                header_file.write("    ")
            header_file.write(f"0x{value:04X}, ")
            if (i + 1) % 10 == 0:
                header_file.write("\n")

        header_file.write("\n};\n\n")
        header_file.write(f"#endif // {base_name}_H\n")

    print(f"Header file written to: {header_filename}")

def main():
    print("Script started. Checking arguments...")

    if len(sys.argv) != 2:
        print("Usage: python png_to_header.py <FileName.png>")
        return

    input_file = sys.argv[1]
    print(f"Input file: {input_file}")

    if not os.path.isfile(input_file):
        print(f"Error: File '{input_file}' not found.")
        return

    try:
        print(f"Opening image: {input_file}")
        img = Image.open(input_file).convert("RGB")
        width, height = img.size
        
        print(f"Image loaded successfully: {width}x{height}")
        print("Converting image data...")

        img_data = [convert_to_565(r, g, b) for r, g, b in img.getdata()]
        print("Conversion complete. Writing header file...")

        write_header_file(input_file, width, height, img_data)
    
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
