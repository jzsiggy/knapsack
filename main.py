from os import listdir, system
from os.path import isfile, join

onlyfiles = [f for f in listdir("./inputs") if isfile(join("./inputs", f))]

for f in onlyfiles:
    system("./greedy/main.out < inputs/{input} > outputs/greedy/{output}".format(input=f, output=f.replace("input", "output")))
    system("./random/main.out < inputs/{input} > outputs/random/{output}".format(input=f, output=f.replace("input", "output")))

# for f in onlyfiles:
#     system("./parallel.out < inputs/{input} > outputs/parallel/{output}".format(input=f, output=f.replace("input", "output")))
#     system("./iterative.out < inputs/{input} > outputs/iterative/{output}".format(input=f, output=f.replace("input", "output")))
#     system("./gpu.out < inputs/{input} > outputs/gpu/{output}".format(input=f, output=f.replace("input", "output")))