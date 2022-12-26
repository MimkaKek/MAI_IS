import sys, os
import subprocess

if len(sys.argv) > 1:
    roots = []
    for arg in sys.argv:
        if arg == sys.argv[0]:
            continue
        roots.append(arg)

    for root in roots:
        onlyfiles = [f for f in os.listdir(root) if os.path.isfile(os.path.join(root, f))]
        for file in onlyfiles:
            subprocess.run(["python", "./extract_text_from_html.py", root + "/" + file])
        