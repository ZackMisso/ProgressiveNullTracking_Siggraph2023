import sys
import matplotlib.pyplot as plt

import matplotlib as mpl
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams
import numpy as np
import math
import os

from matplotlib.pyplot import figure

from math import atan2, degrees
import numpy as np

def setup():
    plt.style.use("ggplot")
    textColor = '#444444'
    fontName = 'Linux Biolinium'
    mpl.rc('font', size=30)
    mpl.rc('font', weight='light')
    mpl.rc('text', hinting='native')
    siz = 22.5  # 7.97
    pgf_with_latex = {
        "pgf.texsystem": "pdflatex",
        "text.usetex": True,
        "font.family": "serif",
        "font.serif": [],
        "font.sans-serif": [],
        "font.monospace": [],
        "axes.titlesize":  siz,
        "axes.labelsize":  siz,
        "font.size":  siz,
        "legend.fontsize":  siz,
        "xtick.labelsize":  siz,
        "ytick.labelsize":  siz,
        "pgf.preamble": r"\usepackage[utf8x]{inputenc} \usepackage[T1]{fontenc}"
    }
    mpl.rcParams.update(pgf_with_latex)

os.system("mkdir ../../results/")
os.system("mkdir ../../results/figure_2/")
os.system("mkdir ../../results/figure_2/data")

trials = 100
base_path = "../../results/figure_2/data/"
path_to_results = "../../results/figure_2/"

########################### EXTINCTION GRAPHS ########################

def gen_ext_graph(plot_non):
    figure(figsize=(6, 3), dpi=80)

    setup()

    plt.xlabel("x")
    plt.ylabel("Extinction")

    ext_xs = []
    ext_vals = []

    ext_data = open(base_path + "ext_func.txt").readlines()

    for j in range(0, int(len(ext_data) / 2)):
        ext_xs.append(float(ext_data[2*j]))
        ext_vals.append(float(ext_data[2*j+1]))

    plt.plot(ext_xs, ext_vals, "-", linewidth=5.0, label="$\\mu_r(x)$")

    maj_xs = []
    maj_vals = []

    if plot_non == False:
        maj_xs.append(0.0)
        maj_xs.append(5.0)
        maj_vals.append(3.5)
        maj_vals.append(3.5)
    else:
        maj_xs.append(0.0)
        maj_xs.append(5.0)
        maj_vals.append(0.4)
        maj_vals.append(0.4)

    plt.plot(maj_xs, maj_vals, "-b", linewidth=2.0, label="$\\mu_t$")

    # the fill
    if (plot_non == False):
        index = 0
        fill_xs = []
        fill_y1 = []
        fill_y2 = []

        while ext_xs[index] < 5.0:
            fill_xs.append(ext_xs[index])
            fill_y1.append(ext_vals[index])
            fill_y2.append(3.5)
            index = index + 1

        matplotlib.pyplot.fill_between(fill_xs, fill_y1, fill_y2, alpha = 0.2, color="g", label="$\\mu_t \geq \\mu_r(x)$")

        fill_xs = []
        fill_y1 = []
        fill_y2 = []

    else:
        index = 0
        fill_xs = []
        fill_y1 = []
        fill_y2 = []

        while ext_xs[index] < 1.0:
            fill_xs.append(ext_xs[index])
            fill_y1.append(ext_vals[index])
            fill_y2.append(0.4)
            index = index + 1

        matplotlib.pyplot.fill_between(fill_xs, fill_y1, fill_y2, alpha = 0.2, color="g")

        fill_xs = []
        fill_y1 = []
        fill_y2 = []

        while ext_xs[index] < 5.0:
            fill_xs.append(ext_xs[index])
            fill_y1.append(ext_vals[index])
            fill_y2.append(0.4)
            index = index + 1

        matplotlib.pyplot.fill_between(fill_xs, fill_y1, fill_y2, alpha = 0.2, label="$\\mu_t \leq \\mu_r(x)$")

    plt.ylim(0.0, 3.7)
    plt.xlim(0.0, 3.0)
    plt.legend()

    # plt.tight_layout(rect=[-0.041, -0.095, 1.043, 1.082])
    plt.tight_layout(rect=[-0.055, -0.13, 1.058, 1.11])

    if plot_non == True:
        plt.savefig(path_to_results + "non_rat_ext_full.png")
        plt.savefig(path_to_results + "non_rat_ext_full.pdf")
        plt.savefig(path_to_results + "non_rat_ext_full.pgf")
    else:
        plt.savefig(path_to_results + "rat_ext_full.png")
        plt.savefig(path_to_results + "rat_ext_full.pdf")
        plt.savefig(path_to_results + "rat_ext_full.pgf")
    plt.clf()

gen_ext_graph(False)
gen_ext_graph(True)

def gen_graph(plot_non):
    figure(figsize=(6, 3), dpi=80)

    setup()

    plt.xlabel("x")
    plt.ylabel("Transmittance")

    for i in range(0, trials):
        rat_xs = []
        rat_vals = []

        rat_data = []
        if plot_non == True:
            rat_data = open(base_path + str(i) + "/non_rat.txt").readlines()
        else:
            rat_data = open(base_path + str(i) + "/rat.txt").readlines()

        for j in range(0, int(len(rat_data) / 2)):
            rat_xs.append(float(rat_data[2*j]))
            rat_vals.append(float(rat_data[2*j+1]))
        
        plt.plot(rat_xs, rat_vals, "-b", alpha=0.03)

    trans_xs = []
    trans_vals = []

    trans_data = open(base_path + "trans_func.txt").readlines()

    for j in range(0, int(len(trans_data) / 2)):
        trans_xs.append(float(trans_data[2*j]))
        trans_vals.append(float(trans_data[2*j+1]))

    plt.plot(trans_xs, trans_vals, "-", linewidth=5.0)

    avg_xs = []
    avg_vals = []

    avg_data = []
    if plot_non == True:
        avg_data = open(base_path + "avg_non_rat.txt").readlines()
    else:
        avg_data = open(base_path + "avg_rat.txt").readlines()

    for j in range(0, int(len(avg_data) / 2)):
        avg_xs.append(float(avg_data[2*j]))
        avg_vals.append(float(avg_data[2*j+1]))

    plt.plot(avg_xs, avg_vals, "-k", linewidth=2.0)

    # if plot_non == True:
    #     plt.ylim(-5.0, 5.0)
    # else:
    #     plt.ylim(0.0, 1.0)

    plt.ylim(-1.0, 2.0)
    plt.xlim(0.0, 3.0)

    plt.tight_layout(rect=[-0.055, -0.13, 1.058, 1.11])
    if plot_non == True:
        plt.savefig(path_to_results + "non_rat_full.png")
        plt.savefig(path_to_results + "non_rat_full.pdf")
        plt.savefig(path_to_results + "non_rat_full.pgf")
    else:
        plt.savefig(path_to_results + "rat_full.png")
        plt.savefig(path_to_results + "rat_full.pdf")
        plt.savefig(path_to_results + "rat_full.pgf")
    plt.clf()

gen_graph(False)
gen_graph(True)