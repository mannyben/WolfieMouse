import re, sys

cc_str = r'''
/*******************************************************************************
 * This file is auto-generated by mazeTextToCCString.py
 ******************************************************************************/

#include "Maze.hpp"

const Maze::StringMaze mazeString = Maze::StringMaze{
.buf =
%%string_here%%
};
'''

re_str = ""
try:
    f = open(sys.argv[1])
    for line in f.readlines():
        line = '"' + line[:-1] + '\\\\n' + '"\n'
        re_str += line
except Exception as e:
    print(e)
    exit()

regex = re.compile(r'\%\%string_here\%\%')
cc_str = regex.sub(re_str, cc_str)

print(cc_str)