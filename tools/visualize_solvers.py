#!/usr/bin/env python3

import utils

def output_perfect_problems(config):
    with open('perfect.csv', 'w') as f:
        pass

def output_average_resemblance(config):
    with open('average.csv', 'w') as f:
        pass

def output_median_resemblance(config):
    with open('median.csv', 'w') as f:
        pass

def output_total_resemblance(config):
    with open('total.csv', 'w') as f:
        pass

def main():
    configs = utils.load_all_solvers_config()
    output_perfect_problems(config)
    output_average_resemblance(config)
    output_median_resemblance(config)
    output_total_resemblance(config)

if __name__ == '__main__':
    main()

