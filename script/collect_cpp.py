import os
import sys
import shutil

repository = sys.argv[1]

for root, dirs, files in os.walk(repository):
	path = root.split(os.sep)
	for file in files:
		if(file.endswith('.cpp') or file.endswith('.hpp')):
			src = os.path.join(root,file)
			dst = "../src"
			print src
			shutil.copy2(src,dst)