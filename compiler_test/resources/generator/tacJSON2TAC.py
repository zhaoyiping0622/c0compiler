import argparse
import json

if __name__ == "__main__":
    args = argparse.ArgumentParser()
    args.add_argument("input", type=str)
    args.add_argument("output", type=str)
    args = args.parse_args()
    with open(args.input)as f:
        j = json.load(f)

    with open(args.output, "w")as f:
        for t in j:
            print(t, file=f)
