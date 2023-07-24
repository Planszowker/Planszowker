import shutil
import os
import sys
import zipfile

GAMES_PATH = '../planszowker_server/scripts/games/'


def make_zipfile(output_filename, source_dir):
    with zipfile.ZipFile(output_filename, "w") as zip_file:
        for root, dirs, files in os.walk(source_dir):
            zip_file.write(root, os.path.relpath(root, source_dir))
            for file in files:
                filename = os.path.join(root, file)
                if os.path.isfile(filename):  # regular files only
                    zip_file.write(filename, os.path.join(os.path.relpath(root, source_dir), file))


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("You have to specify output folder!")
        exit(-1)

    output_path = sys.argv[1]
    os.chdir(sys.path[0])

    for item in os.listdir(GAMES_PATH):
        make_zipfile(output_path + '/' + item + '.plagame', GAMES_PATH + item)
