# import setup
import sys
import matplotlib.pyplot as plt
# import setup

import matplotlib as mpl
# import matplotlib.pyplot as plt
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams
import numpy as np
import math
# from labellines import labelLines

from matplotlib.pyplot import figure

from math import atan2, degrees
import numpy as np

def setup():
    plt.style.use("ggplot")
    textColor = '#444444'
    fontName = 'Linux Biolinium'
    #mpl.rc('font', family=fontName)
    mpl.rc('font', size=30)
    mpl.rc('font', weight='light')
    #mpl.rc('text', color=textColor)
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
        # "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
        "pgf.preamble": r"\usepackage[utf8x]{inputenc} \usepackage[T1]{fontenc}"
    }
    mpl.rcParams.update(pgf_with_latex)

figure(figsize=(8, 4), dpi=80)

setup()

base_path = "/Users/corneria/Documents/Research/bias_research/progressive_controls/6398d9dc531c9eefb55fa6a3/figures/bunny/"
path_to_results = base_path

total = float(80*80*80)
# total = float(118318)
bounding = [39862, 33107, 27849, 23818, 20560, 18016, 16178, 14729, 13693, 12967, 12395, 11999, 11689]
xs = [1,2,4,8,16,32,64,128,256,512,1024,2048,4096]

final = []

plt.ylabel("Percent Bounding")
plt.xlabel("spp")

for val in bounding:
    final.append(1.0 - val / total)

# trials = 100
# base_path = "/Users/corneria/Documents/Research/bias_research/progressive_controls/6398d9dc531c9eefb55fa6a3/figures/bunny/"
# path_to_results = base_path

plt.plot([0], [0.96])
plt.plot([0], [0.96])
plt.plot(xs, final, linewidth=5.0)

# plt.plot(rat_xs, rat_vals, "-b", alpha=0.03)

# plt.legend()

plt.xlim(8, 1024)

plt.xscale('log', base=2)
# plt.yscale('log', base=2)

plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.savefig(path_to_results + "proportion.png")
plt.savefig(path_to_results + "proportion.pdf")
plt.savefig(path_to_results + "proportion.pgf")
plt.clf()

