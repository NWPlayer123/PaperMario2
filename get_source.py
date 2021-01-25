import os

for root, dirs, files in os.walk("source"):
    for entry in files:
        print("\t%s/%s" % (root.replace("\\", "/"), entry))
    print
