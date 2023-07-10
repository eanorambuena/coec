import re, subprocess, sys

def transpile (INPUT_PATH, OUTPUT_PATH):
    with open(INPUT_PATH, "r") as f:
        text = f.read()

    text = text.replace("\t", "")
    text = text.replace("  ", "") 

    text = re.sub(r"property\s*(\w+)", "PROPERTY(\\1)", text)
    text = re.sub(r"method\s*(\w+)", "METHOD(\\1)", text)

    text = re.sub(r"(class\s+)(\w+)(\s*\(\s*)(\w+)(\s*\)\s*:)", "EXTEND(\\4,\\2);\n#undef SELF\n#define SELF \\2", text)
    text = re.sub(r"(class\s+)(\w+)(\s*:)", "CLASS(\\2);\n#undef SELF\n#define SELF \\2", text)
    text = re.sub(r"\)\s*{", "){BEGIN;", text)
    text = re.sub(r"return", "END;return", text)
    text = re.sub(r"yield;", "int* RETURN = calloc(1, sizeof(int));END;YIELD;", text)
    text = re.sub(r"yield\s*([^;]+);", "int* RETURN = calloc(1, sizeof(int));*RETURN=\\1;END;YIELD;", text)

    text = re.sub(r"!(\w+)\s+(\w+).(\w+)\s*=([^;]+);", "\\1_\\2_\\3 = \\4;\\2.SET(\\3,&\\1_\\2_\\3);", text)
    text = re.sub(r"(\w+)\s+(\w+).(\w+)\s*=([^;]+);", "\\1 \\1_\\2_\\3 = \\4;\\2.SET(\\3,&\\1_\\2_\\3);", text)

    text = re.sub(r"(\w+).SET\s*\(", "SET(\\1,", text)
    text = re.sub(r"(\w+).GET\s*\(", "GET(\\1,", text)
    text = re.sub(r"(\w+).CALL\s*\(", "CALL(\\1,", text)


    with open("coec/class.h", "r") as f:
        includes = f.read()

    with open("coec/class.c", "r") as f:
        includes += f.read().replace('#include \"class.h\"', "")

    with open(OUTPUT_PATH, "w") as f:
        f.write(includes+ '\n' + text)

def main (argv, argc, use_gcc = True):
    if argc < 2:
        print("Usage: coec.py <input> <output> [executable]")
        return
    
    INPUT_PATH = argv[1]
    OUTPUT_PATH = argv[2]
    assert INPUT_PATH.endswith(".coec"), "Input file must be a .c file"
    assert OUTPUT_PATH.endswith(".c"), "Output file must be a .c file"
    transpile(INPUT_PATH, OUTPUT_PATH)

    if not use_gcc:
        return
    
    DEFAULT_EXECUTABLE = OUTPUT_PATH[:-2].split("/")[-1].split("\\")[-1]

    if argc > 3:
        if argv[3].startswith("--execute") or argv[3].startswith("-e"):
            subprocess.call(["gcc", OUTPUT_PATH, "-o", DEFAULT_EXECUTABLE])
            subprocess.call(["./" + DEFAULT_EXECUTABLE])
        else:
            subprocess.call(["gcc", OUTPUT_PATH, "-o", argv[3]])
    if argc > 4:
        subprocess.call(["gcc", OUTPUT_PATH, "-o", argv[3]])
        if argv[4].startswith("--execute") or argv[4].startswith("-e"):
            subprocess.call(["./" + argv[3]])

if __name__ == "__main__":
    main(sys.argv, len(sys.argv))
