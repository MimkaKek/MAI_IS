from sys import argv, stdout
import os
import time


def main():
    if len(argv) > 1:
        roots = []
        for arg in argv:
            if arg == argv[0]:
                continue
            roots.append(arg)
        
        total_len = 0
        count_token = 0
        avg = 0.0

        for root in roots:
            if not os.path.exists(os.path.join(root, "data")):
                os.makedirs(os.path.join(root, "data"))

            begin_time = time.time()
            tm_hour    = 0
            tm_min     = 0
            tm_sec     = 0
            onlyfiles = [f for f in os.listdir(os.path.join(root, "data")) if os.path.isfile(os.path.join(root, "data", f))]
            for filename in onlyfiles:
                delta_time = time.gmtime(time.time() - begin_time)
                tm_hour    = delta_time.tm_hour
                tm_min     = delta_time.tm_min
                tm_sec     = delta_time.tm_sec
                print("\33[2K\r| Total time: {hour:02d}:{min:02d}:{sec:02d} | Tokens = {tokens:10d}, Avg_len  {agv_len:10.5f} | Check {path}...".format(hour=tm_hour, min=tm_min, sec=tm_sec, path=os.path.join(root, filename), tokens=count_token, agv_len=avg), end="")
                with open(os.path.join(root, "data", filename), "r") as file:
                    i = 0
                    for line in file:
                        if i == 0:
                            i = 1
                            continue
                        if i % 2 == 1:
                            count_token += 1
                            total_len   += len(line)
                            avg = total_len / count_token
                            stdout.flush()
                        i += 1
            
            avg = total_len / count_token
            print("\33[2K\r| Total time: {hour:02d}:{min:02d}:{sec:02d} | Tokens = {tokens:10d}, Avg_len  {agv_len:10.5f}".format(hour=tm_hour, min=tm_min, sec=tm_sec, tokens=count_token, agv_len=avg))
            print("========================")

    else:
        print("Example usage: python " + argv[0] + " [dir_path...]")

if __name__ == "__main__":
    main()