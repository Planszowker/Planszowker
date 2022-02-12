import shutil
import os
import sys

GAMES_PATH = '../PlanszowkerServer/lua-scripts/games/'

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("You have to specify output folder!")
        exit(-1)

    output_path = sys.argv[1]

    os.chdir(sys.path[0])

    for item in os.listdir(GAMES_PATH):
        shutil.make_archive(output_path + '/' + item, 'zip', GAMES_PATH + item)
        os.rename(output_path + '/' + item + '.zip', output_path + '/' + item + '.plagame')
