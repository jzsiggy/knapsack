from os import listdir, system
from os.path import isfile, join

onlyfiles = [f for f in listdir("./inputs") if isfile(join("./inputs", f))]

for f in onlyfiles:
    system("./greedy/main.out < inputs/{input} > outputs/greedy/{output}".format(input=f, output=f.replace("input", "output")))
    system("./random/main.out < inputs/{input} > outputs/random/{output}".format(input=f, output=f.replace("input", "output")))
