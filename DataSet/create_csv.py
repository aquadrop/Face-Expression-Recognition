#/usr/bin/env python

import sys
import os.path

# This is a tiny script to help you creating a CSV file from a face
# database with a similar hierarchie:
#
#  philipp@mango:~/facerec/data/at$ tree
#  .
#  |-- README
#  |-- s1
#  |   |-- 1.pgm
#  |   |-- ...
#  |   |-- 10.pgm
#  |-- s2
#  |   |-- 1.pgm
#  |   |-- ...
#  |   |-- 10.pgm
#  ...
#  |-- s40
#  |   |-- 1.pgm
#  |   |-- ...
#  |   |-- 10.pgm
#

if __name__ == "__main__":

    if len(sys.argv) != 2:
        print "usage: create_csv <base_path>"
        sys.exit(1)
    f = open('dataset.csv', 'w')
    
    BASE_PATH=sys.argv[1]
    SEPARATOR=";"

    for dirname, dirnames, filenames in os.walk(BASE_PATH):
        for subdirname in dirnames:
            label = subdirname
            subject_path = os.path.join(dirname, subdirname)
            for filename in os.listdir(subject_path):
                abs_path = "%s/%s" % (subject_path, filename)
                print "%s%s%s" % (abs_path, SEPARATOR, label)
                f.write("%s%s%s\n"%(abs_path, SEPARATOR, label))
    f.close();