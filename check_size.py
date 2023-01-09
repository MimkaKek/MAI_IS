import os
from sys import argv

def main():
    if len(argv) > 1:
        roots = []
        for arg in argv:
            if arg == argv[0]:
                continue
            roots.append(arg)

        total_size = 0
        print("=============================")
        for root in roots:
            if not os.path.exists(os.path.join(root, "data")):
                os.makedirs(os.path.join(root, "data"))

            file_num = 0
            onlyfiles = [f for f in os.listdir(os.path.join(root, "data")) if os.path.isfile(os.path.join(root, "data", f))]
            for file in onlyfiles:
                file_num += 1
                total_size += os.path.getsize(os.path.join(root, "data", file))
                avg_size = total_size / file_num
                print("\33[2K\r| Total size = {total:12d} | Avg size = {avg:6.4f} | Counting file size... {path}".format(total=total_size, avg=avg_size, path=os.path.join(root, "html", file)), end="")
            print("")
        print("=============================")

    else:
        print("Example usage: python " + argv[0] + " [dir_paths...]")

if __name__ == "__main__":
    main()