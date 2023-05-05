#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"

double find_mse_for_cost(double desired_cost,
                         int &last_index,
                         const std::vector<double> &costs,
                         const std::vector<double> &mses)
{
    int index = last_index;
    while (costs[index] < desired_cost)
        index++;

    last_index = index;

    // this is a weird case I'm not sure how to handle
    if (index == 0)
        return mses[index];
    if (index == costs.size())
    {
        return mses[index - 1];
    }

    double proxy = (desired_cost - costs[index - 1]) / (costs[index] - costs[index - 1]);
    return proxy * mses[index] + (1.0 - proxy) * mses[index - 1];
}

double find_err_for_cost(double desired_cost,
                         int &last_index,
                         const std::vector<double> &costs,
                         const std::vector<double> &errs)
{
    int index = last_index;
    while (costs[index] < desired_cost)
        index++;

    last_index = index;

    // this is a weird case I'm not sure how to handle
    if (index == 0)
        return errs[index];
    if (index == costs.size())
    {
        return errs[index - 1];
    }

    double proxy = (desired_cost - costs[index - 1]) / (costs[index] - costs[index - 1]);
    return proxy * errs[index] + (1.0 - proxy) * errs[index - 1];
}

bool failed_test(std::string loc, int index)
{
    return false;

    std::ifstream ppm_file;
    ppm_file.open(loc + "equal_cost_ppm_" + std::to_string(index) + ".txt");

    std::cout << "checking for errors in index: " << index << std::endl;

    double ppm_final_cost = 0.0;

    std::string line_str;
    while (getline(ppm_file, line_str))
    {
        double index = std::stod(line_str);
        getline(ppm_file, line_str);
        ppm_final_cost = std::stod(line_str);
        getline(ppm_file, line_str);
        double mean = std::stod(line_str);
        getline(ppm_file, line_str);
        double mse = std::stod(line_str);
    }

    std::ifstream deb_file;
    deb_file.open(loc + "equal_cost_deb_cone_alpha_final_" + std::to_string(index) + ".txt");

    double deb_final_cost = 0.0;

    while (getline(deb_file, line_str))
    {
        double index = std::stod(line_str);
        getline(deb_file, line_str);
        deb_final_cost = std::stod(line_str);
        getline(deb_file, line_str);
        double mean = std::stod(line_str);
        getline(deb_file, line_str);
        double mse = std::stod(line_str);
    }

    ppm_file.close();
    deb_file.close();

    // if ()

    return ppm_final_cost > deb_final_cost;
}

void mse_run()
{
    int start_index = 0;
    int end_index = 640;

    std::vector<double> ppm_costs = std::vector<double>();
    std::vector<double> ppm_mses = std::vector<double>();
    std::vector<double> deb_uni_mses = std::vector<double>();
    std::vector<double> deb_cone_mses = std::vector<double>();
    // std::vector<double> deb_quad_mses = std::vector<double>();
    // std::vector<double> deb_quart_mses = std::vector<double>();
    std::vector<double> ppm_errs = std::vector<double>();
    std::vector<double> deb_uni_errs = std::vector<double>();
    std::vector<double> deb_cone_errs = std::vector<double>();
    // std::vector<double> deb_quad_errs = std::vector<double>();
    // std::vector<double> deb_quart_errs = std::vector<double>();

    std::string loc = "/Users/corneria/Documents/Research/bias_research/ConvergenceRater/results/photon_deb_vs_ppm/full_test/";

    int actual_index = 0;
    std::vector<int> failed_tests = std::vector<int>();

    // parse and process the ppm results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        if (failed_test(loc, i))
        {
            failed_tests.push_back(i);
            std::cout << "run failed: " << i << std::endl;
            continue;
        }

        std::cout << "incorporating ppm results from run: " << i << std::endl;
        std::ifstream ppm_file;
        ppm_file.open(loc + "equal_cost_ppm_" + std::to_string(i) + ".txt");
        std::cout << loc + "equal_cost_ppm_" + std::to_string(i) + ".txt" << std::endl;

        int cnt = 0;

        std::string line_str;
        while (getline(ppm_file, line_str))
        {
            // std::cout << "cnt " << cnt << std::endl;
            double index = std::stod(line_str);
            getline(ppm_file, line_str);
            double cost = std::stod(line_str);
            getline(ppm_file, line_str);
            double err = std::stod(line_str);
            getline(ppm_file, line_str);
            double mse = std::stod(line_str);

            if (i == start_index)
            {
                ppm_costs.push_back(cost);
                ppm_mses.push_back(mse);
                ppm_errs.push_back(err);
            }
            else
            {
                ppm_costs[cnt] = ppm_costs[cnt] + 1.0 / double(actual_index + 1) * (cost - ppm_costs[cnt]);
                ppm_mses[cnt] = ppm_mses[cnt] + 1.0 / double(actual_index + 1) * (mse - ppm_mses[cnt]);
                ppm_errs[cnt] = ppm_errs[cnt] + 1.0 / double(actual_index + 1) * (err - ppm_errs[cnt]);
                cnt++;
            }
        }

        actual_index++;
    }

    // write the ppm results
    std::ofstream ppm_res;
    ppm_res.open(loc + "avg_equal_cost_ppm.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        ppm_res << ppm_costs[i] << std::endl;
        ppm_res << ppm_mses[i] << std::endl;
        ppm_res << ppm_errs[i] << std::endl;
        deb_uni_mses.push_back(0.0);
        // deb_quad_mses.push_back(0.0);
        deb_cone_mses.push_back(0.0);
        // deb_quart_mses.push_back(0.0);
        deb_uni_errs.push_back(0.0);
        // deb_quad_errs.push_back(0.0);
        deb_cone_errs.push_back(0.0);
        // deb_quart_errs.push_back(0.0);
    }
    ppm_res.close();

    // DEBIASED UNIFORM //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating uni deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_uni_square_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        // int cnt = 0;

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        // std::cout << "parsed the thing" << std::endl;
        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_uni_mses[k] = deb_uni_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_uni_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_uni_errs[k] = deb_uni_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_uni_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_res;
    deb_res.open(loc + "avg_equal_cost_deb_uni_square.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_res << ppm_costs[i] << std::endl;
        deb_res << deb_uni_mses[i] << std::endl;
        deb_res << deb_uni_errs[i] << std::endl;
    }
    deb_res.close();
    // DEBIASED UNIFORM //

    // DEBIASED CONE //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating cone deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_cone_square_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        // int cnt = 0;

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        // std::cout << "parsed the thing" << std::endl;
        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_cone_mses[k] = deb_cone_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_cone_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_cone_errs[k] = deb_cone_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_cone_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_cone_res;
    deb_cone_res.open(loc + "avg_equal_cost_deb_cone_square.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_cone_res << ppm_costs[i] << std::endl;
        deb_cone_res << deb_cone_mses[i] << std::endl;
        deb_cone_res << deb_cone_errs[i] << std::endl;
    }
    deb_cone_res.close();
    // DEBIASED UNIFORM //

    // // DEBIASED QUAD //
    // actual_index = 0;
    // // parse and process the deb results for index i
    // for (int i = start_index; i < end_index; ++i)
    // {
    //     for (int k = 0; k < failed_tests.size(); ++k)
    //     {
    //         if (failed_tests[k] == i)
    //             continue;
    //     }

    //     std::cout << "incorporating quad deb results from run: " << i << std::endl;
    //     std::ifstream deb_file;
    //     deb_file.open(loc + "equal_cost_deb_cone_square_" + std::to_string(i) + ".txt");

    //     std::vector<double> costs = std::vector<double>();
    //     std::vector<double> mses = std::vector<double>();

    //     // int cnt = 0;

    //     std::string line_str;
    //     while (getline(deb_file, line_str))
    //     {
    //         double index = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double cost = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double mean = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double mse = std::stod(line_str);

    //         costs.push_back(cost);
    //         mses.push_back(mse);
    //     }

    //     // std::cout << "parsed the thing" << std::endl;
    //     int last_index = 0;

    //     for (int k = 0; k < ppm_costs.size(); ++k)
    //     {
    //         double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
    //         deb_quad_mses[k] = deb_quad_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_quad_mses[k]);
    //     }

    //     actual_index++;
    // }

    // // write the deb results
    // std::ofstream deb_quad_res;
    // deb_quad_res.open(loc + "avg_equal_cost_deb_quad_square.txt");
    // for (int i = 0; i < ppm_costs.size(); ++i)
    // {
    //     deb_quad_res << ppm_costs[i] << std::endl;
    //     deb_quad_res << deb_cone_mses[i] << std::endl;
    // }
    // deb_quad_res.close();
    // // DEBIASED QUAD //

    std::cout << "TOTAL FAILED TESTS: " << failed_tests.size() << std::endl;
}

void opt_run()
{
    int start_index = 0;
    int end_index = 120;

    std::vector<double> ppm_costs = std::vector<double>();
    std::vector<double> ppm_mses = std::vector<double>();
    std::vector<double> deb_sqr_mses = std::vector<double>();
    std::vector<double> deb_alp_mses = std::vector<double>();
    std::vector<double> deb_inf_mses = std::vector<double>();

    std::vector<double> ppm_errs = std::vector<double>();
    std::vector<double> deb_sqr_errs = std::vector<double>();
    std::vector<double> deb_alp_errs = std::vector<double>();
    std::vector<double> deb_inf_errs = std::vector<double>();

    std::string loc = "/Users/corneria/Documents/Research/bias_research/ConvergenceRater/results/photon_deb_vs_ppm/full_test/";

    int actual_index = 0;
    std::vector<int> failed_tests = std::vector<int>();

    // parse and process the ppm results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        if (failed_test(loc, i))
        {
            failed_tests.push_back(i);
            std::cout << "run failed: " << i << std::endl;
            continue;
        }

        std::cout << "incorporating ppm results from run: " << i << std::endl;
        std::ifstream ppm_file;
        ppm_file.open(loc + "equal_cost_ppm_" + std::to_string(i) + ".txt");
        std::cout << loc + "equal_cost_ppm_" + std::to_string(i) + ".txt" << std::endl;

        int cnt = 0;

        std::string line_str;
        while (getline(ppm_file, line_str))
        {
            // std::cout << "cnt " << cnt << std::endl;
            double index = std::stod(line_str);
            getline(ppm_file, line_str);
            double cost = std::stod(line_str);
            getline(ppm_file, line_str);
            double err = std::stod(line_str);
            getline(ppm_file, line_str);
            double mse = std::stod(line_str);

            if (i == start_index)
            {
                ppm_costs.push_back(cost);
                ppm_mses.push_back(mse);
                ppm_errs.push_back(err);
            }
            else
            {
                ppm_costs[cnt] = ppm_costs[cnt] + 1.0 / double(actual_index + 1) * (cost - ppm_costs[cnt]);
                ppm_mses[cnt] = ppm_mses[cnt] + 1.0 / double(actual_index + 1) * (mse - ppm_mses[cnt]);
                ppm_errs[cnt] = ppm_errs[cnt] + 1.0 / double(actual_index + 1) * (err - ppm_errs[cnt]);
                cnt++;
            }
        }

        actual_index++;
    }

    // write the ppm results
    std::ofstream ppm_res;
    ppm_res.open(loc + "avg_equal_cost_ppm.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        ppm_res << ppm_costs[i] << std::endl;
        ppm_res << ppm_mses[i] << std::endl;
        ppm_res << ppm_errs[i] << std::endl;
        deb_sqr_mses.push_back(0.0);
        deb_alp_mses.push_back(0.0);
        deb_inf_mses.push_back(0.0);
        deb_sqr_errs.push_back(0.0);
        deb_alp_errs.push_back(0.0);
        deb_inf_errs.push_back(0.0);
    }
    ppm_res.close();

    // DEBIASED SQUARED //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating uni deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_cone_square_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_sqr_mses[k] = deb_sqr_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_sqr_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_sqr_errs[k] = deb_sqr_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_sqr_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_res;
    deb_res.open(loc + "avg_equal_cost_deb_cone_square.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_res << ppm_costs[i] << std::endl;
        deb_res << deb_sqr_mses[i] << std::endl;
        deb_res << deb_sqr_errs[i] << std::endl;
    }
    deb_res.close();
    // DEBIASED SQUARED //

    // DEBIASED ALPHA //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating cone deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_cone_alpha_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_alp_mses[k] = deb_alp_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_alp_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_alp_errs[k] = deb_alp_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_alp_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_alp_res;
    deb_alp_res.open(loc + "avg_equal_cost_deb_cone_alpha.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_alp_res << ppm_costs[i] << std::endl;
        deb_alp_res << deb_alp_mses[i] << std::endl;
        deb_alp_res << deb_alp_errs[i] << std::endl;
    }
    deb_alp_res.close();
    // DEBIASED ALPHA //

    // DEBIASED INFINITE //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating cone inf deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_cone_alpha_inf_work_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_inf_mses[k] = deb_inf_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_inf_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_inf_errs[k] = deb_inf_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_inf_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_inf_res;
    deb_inf_res.open(loc + "avg_equal_cost_deb_cone_alpha_inf.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_inf_res << ppm_costs[i] << std::endl;
        deb_inf_res << deb_inf_mses[i] << std::endl;
        deb_inf_res << deb_inf_errs[i] << std::endl;
    }
    deb_inf_res.close();
    // DEBIASED INFINITE //

    // // DEBIASED QUAD //
    // actual_index = 0;
    // // parse and process the deb results for index i
    // for (int i = start_index; i < end_index; ++i)
    // {
    //     for (int k = 0; k < failed_tests.size(); ++k)
    //     {
    //         if (failed_tests[k] == i)
    //             continue;
    //     }

    //     std::cout << "incorporating quad deb results from run: " << i << std::endl;
    //     std::ifstream deb_file;
    //     deb_file.open(loc + "equal_cost_deb_cone_square_" + std::to_string(i) + ".txt");

    //     std::vector<double> costs = std::vector<double>();
    //     std::vector<double> mses = std::vector<double>();

    //     // int cnt = 0;

    //     std::string line_str;
    //     while (getline(deb_file, line_str))
    //     {
    //         double index = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double cost = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double mean = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double mse = std::stod(line_str);

    //         costs.push_back(cost);
    //         mses.push_back(mse);
    //     }

    //     // std::cout << "parsed the thing" << std::endl;
    //     int last_index = 0;

    //     for (int k = 0; k < ppm_costs.size(); ++k)
    //     {
    //         double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
    //         deb_quad_mses[k] = deb_quad_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_quad_mses[k]);
    //     }

    //     actual_index++;
    // }

    // // write the deb results
    // std::ofstream deb_quad_res;
    // deb_quad_res.open(loc + "avg_equal_cost_deb_quad_square.txt");
    // for (int i = 0; i < ppm_costs.size(); ++i)
    // {
    //     deb_quad_res << ppm_costs[i] << std::endl;
    //     deb_quad_res << deb_cone_mses[i] << std::endl;
    // }
    // deb_quad_res.close();
    // // DEBIASED QUAD //

    std::cout << "TOTAL FAILED TESTS: " << failed_tests.size() << std::endl;
}

void alp_compare_run()
{
    int start_index = 0;
    int end_index = 120;

    std::vector<double> ppm_costs = std::vector<double>();
    std::vector<double> ppm_mses = std::vector<double>();
    std::vector<double> deb_sqr_mses = std::vector<double>();
    std::vector<double> deb_alp_mses = std::vector<double>();
    std::vector<double> deb_inf_mses = std::vector<double>();

    std::vector<double> ppm_errs = std::vector<double>();
    std::vector<double> deb_sqr_errs = std::vector<double>();
    std::vector<double> deb_alp_errs = std::vector<double>();
    std::vector<double> deb_inf_errs = std::vector<double>();

    std::string loc = "/Users/corneria/Documents/Research/bias_research/ConvergenceRater/results/photon_deb_vs_ppm/full_test/";

    int actual_index = 0;
    std::vector<int> failed_tests = std::vector<int>();

    // parse and process the ppm results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        if (failed_test(loc, i))
        {
            failed_tests.push_back(i);
            std::cout << "run failed: " << i << std::endl;
            continue;
        }

        std::cout << "incorporating ppm results from run: " << i << std::endl;
        std::ifstream ppm_file;
        ppm_file.open(loc + "equal_cost_ppm_" + std::to_string(i) + ".txt");
        std::cout << loc + "equal_cost_ppm_" + std::to_string(i) + ".txt" << std::endl;

        int cnt = 0;

        std::string line_str;
        while (getline(ppm_file, line_str))
        {
            // std::cout << "cnt " << cnt << std::endl;
            double index = std::stod(line_str);
            getline(ppm_file, line_str);
            double cost = std::stod(line_str);
            getline(ppm_file, line_str);
            double err = std::stod(line_str);
            getline(ppm_file, line_str);
            double mse = std::stod(line_str);

            if (i == start_index)
            {
                ppm_costs.push_back(cost);
                ppm_mses.push_back(mse);
                ppm_errs.push_back(err);
            }
            else
            {
                ppm_costs[cnt] = ppm_costs[cnt] + 1.0 / double(actual_index + 1) * (cost - ppm_costs[cnt]);
                ppm_mses[cnt] = ppm_mses[cnt] + 1.0 / double(actual_index + 1) * (mse - ppm_mses[cnt]);
                ppm_errs[cnt] = ppm_errs[cnt] + 1.0 / double(actual_index + 1) * (err - ppm_errs[cnt]);
                cnt++;
            }
        }

        actual_index++;
    }

    // write the ppm results
    std::ofstream ppm_res;
    ppm_res.open(loc + "avg_equal_cost_ppm.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        ppm_res << ppm_costs[i] << std::endl;
        ppm_res << ppm_mses[i] << std::endl;
        ppm_res << ppm_errs[i] << std::endl;
        deb_sqr_mses.push_back(0.0);
        deb_alp_mses.push_back(0.0);
        deb_inf_mses.push_back(0.0);
        deb_sqr_errs.push_back(0.0);
        deb_alp_errs.push_back(0.0);
        deb_inf_errs.push_back(0.0);
    }
    ppm_res.close();

    // DEBIASED SQUARED //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating uni deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_cone_alpha_50_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_sqr_mses[k] = deb_sqr_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_sqr_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_sqr_errs[k] = deb_sqr_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_sqr_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_res;
    deb_res.open(loc + "avg_equal_cost_deb_cone_alpha_50.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_res << ppm_costs[i] << std::endl;
        deb_res << deb_sqr_mses[i] << std::endl;
        deb_res << deb_sqr_errs[i] << std::endl;
    }
    deb_res.close();
    // DEBIASED SQUARED //

    // DEBIASED ALPHA //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating cone deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_cone_alpha_67_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_alp_mses[k] = deb_alp_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_alp_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_alp_errs[k] = deb_alp_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_alp_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_alp_res;
    deb_alp_res.open(loc + "avg_equal_cost_deb_cone_alpha_67.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_alp_res << ppm_costs[i] << std::endl;
        deb_alp_res << deb_alp_mses[i] << std::endl;
        deb_alp_res << deb_alp_errs[i] << std::endl;
    }
    deb_alp_res.close();
    // DEBIASED ALPHA //

    // DEBIASED INFINITE //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating cone inf deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_cone_alpha_80_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_inf_mses[k] = deb_inf_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_inf_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_inf_errs[k] = deb_inf_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_inf_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_inf_res;
    deb_inf_res.open(loc + "avg_equal_cost_deb_cone_alpha_80.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_inf_res << ppm_costs[i] << std::endl;
        deb_inf_res << deb_inf_mses[i] << std::endl;
        deb_inf_res << deb_inf_errs[i] << std::endl;
    }
    deb_inf_res.close();
    // DEBIASED INFINITE //

    std::cout << "TOTAL FAILED TESTS: " << failed_tests.size() << std::endl;
}

void final_run()
{
    int start_index = 0;
    int end_index = 512;

    std::vector<double> ppm_costs = std::vector<double>();
    std::vector<double> ppm_mses = std::vector<double>();
    std::vector<double> deb_sqr_mses = std::vector<double>();
    std::vector<double> deb_alp_mses = std::vector<double>();
    std::vector<double> deb_inf_mses = std::vector<double>();

    std::vector<double> ppm_errs = std::vector<double>();
    std::vector<double> deb_sqr_errs = std::vector<double>();
    std::vector<double> deb_alp_errs = std::vector<double>();
    std::vector<double> deb_inf_errs = std::vector<double>();

    std::string loc = "/Users/corneria/Documents/Research/bias_research/ConvergenceRater/results/photon_deb_vs_ppm/full_test/";

    int actual_index = 0;
    std::vector<int> failed_tests = std::vector<int>();

    // parse and process the ppm results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        if (failed_test(loc, i))
        {
            failed_tests.push_back(i);
            std::cout << "run failed: " << i << std::endl;
            continue;
        }

        std::cout << "incorporating ppm results from run: " << i << std::endl;
        std::ifstream ppm_file;
        ppm_file.open(loc + "equal_cost_ppm_" + std::to_string(i) + ".txt");
        std::cout << loc + "equal_cost_ppm_" + std::to_string(i) + ".txt" << std::endl;

        int cnt = 0;

        std::string line_str;
        while (getline(ppm_file, line_str))
        {
            // std::cout << "cnt " << cnt << std::endl;
            double index = std::stod(line_str);
            getline(ppm_file, line_str);
            double cost = std::stod(line_str);
            getline(ppm_file, line_str);
            double err = std::stod(line_str);
            getline(ppm_file, line_str);
            double mse = std::stod(line_str);

            if (i == start_index)
            {
                ppm_costs.push_back(cost);
                ppm_mses.push_back(mse);
                ppm_errs.push_back(err);
            }
            else
            {
                ppm_costs[cnt] = ppm_costs[cnt] + 1.0 / double(actual_index + 1) * (cost - ppm_costs[cnt]);
                ppm_mses[cnt] = ppm_mses[cnt] + 1.0 / double(actual_index + 1) * (mse - ppm_mses[cnt]);
                ppm_errs[cnt] = ppm_errs[cnt] + 1.0 / double(actual_index + 1) * (err - ppm_errs[cnt]);
                cnt++;
            }
        }

        actual_index++;
    }

    // write the ppm results
    std::ofstream ppm_res;
    ppm_res.open(loc + "avg_equal_cost_ppm.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        ppm_res << ppm_costs[i] << std::endl;
        ppm_res << ppm_mses[i] << std::endl;
        ppm_res << ppm_errs[i] << std::endl;
        deb_sqr_mses.push_back(0.0);
        deb_alp_mses.push_back(0.0);
        deb_inf_mses.push_back(0.0);
        deb_sqr_errs.push_back(0.0);
        deb_alp_errs.push_back(0.0);
        deb_inf_errs.push_back(0.0);
    }
    ppm_res.close();

    // DEBIASED SQUARED //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating uni deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_cone_alpha_final_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_sqr_mses[k] = deb_sqr_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_sqr_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_sqr_errs[k] = deb_sqr_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_sqr_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_res;
    deb_res.open(loc + "avg_equal_cost_deb_cone_alpha_final.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_res << ppm_costs[i] << std::endl;
        deb_res << deb_sqr_mses[i] << std::endl;
        deb_res << deb_sqr_errs[i] << std::endl;
    }
    deb_res.close();
    // DEBIASED SQUARED //

    // DEBIASED SQUARED //
    actual_index = 0;
    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating uni deb prog results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_debprog_cone_alpha_final_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> mses = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            mses.push_back(mse);
            errs.push_back(err);
        }

        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
            deb_sqr_mses[k] = deb_sqr_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_sqr_mses[k]);
            eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_sqr_errs[k] = deb_sqr_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_sqr_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream debprog_res;
    debprog_res.open(loc + "avg_equal_cost_debprog_cone_alpha_final.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        debprog_res << ppm_costs[i] << std::endl;
        debprog_res << deb_sqr_mses[i] << std::endl;
        debprog_res << deb_sqr_errs[i] << std::endl;
    }
    debprog_res.close();
    // DEBIASED SQUARED //

    // // DEBIASED ALPHA //
    // actual_index = 0;
    // // parse and process the deb results for index i
    // for (int i = start_index; i < end_index; ++i)
    // {
    //     for (int k = 0; k < failed_tests.size(); ++k)
    //     {
    //         if (failed_tests[k] == i)
    //             continue;
    //     }

    //     std::cout << "incorporating cone deb results from run: " << i << std::endl;
    //     std::ifstream deb_file;
    //     deb_file.open(loc + "equal_cost_deb_cone_alpha_67_" + std::to_string(i) + ".txt");

    //     std::vector<double> costs = std::vector<double>();
    //     std::vector<double> mses = std::vector<double>();
    //     std::vector<double> errs = std::vector<double>();

    //     std::string line_str;
    //     while (getline(deb_file, line_str))
    //     {
    //         double index = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double cost = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double err = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double mse = std::stod(line_str);

    //         costs.push_back(cost);
    //         mses.push_back(mse);
    //         errs.push_back(err);
    //     }

    //     int last_index = 0;

    //     for (int k = 0; k < ppm_costs.size(); ++k)
    //     {
    //         double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
    //         deb_alp_mses[k] = deb_alp_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_alp_mses[k]);
    //         eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
    //         deb_alp_errs[k] = deb_alp_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_alp_errs[k]);
    //     }

    //     actual_index++;
    // }

    // // write the deb results
    // std::ofstream deb_alp_res;
    // deb_alp_res.open(loc + "avg_equal_cost_deb_cone_alpha_67.txt");
    // for (int i = 0; i < ppm_costs.size(); ++i)
    // {
    //     deb_alp_res << ppm_costs[i] << std::endl;
    //     deb_alp_res << deb_alp_mses[i] << std::endl;
    //     deb_alp_res << deb_alp_errs[i] << std::endl;
    // }
    // deb_alp_res.close();
    // // DEBIASED ALPHA //

    // // DEBIASED INFINITE //
    // actual_index = 0;
    // // parse and process the deb results for index i
    // for (int i = start_index; i < end_index; ++i)
    // {
    //     for (int k = 0; k < failed_tests.size(); ++k)
    //     {
    //         if (failed_tests[k] == i)
    //             continue;
    //     }

    //     std::cout << "incorporating cone inf deb results from run: " << i << std::endl;
    //     std::ifstream deb_file;
    //     deb_file.open(loc + "equal_cost_deb_cone_alpha_80_" + std::to_string(i) + ".txt");

    //     std::vector<double> costs = std::vector<double>();
    //     std::vector<double> mses = std::vector<double>();
    //     std::vector<double> errs = std::vector<double>();

    //     std::string line_str;
    //     while (getline(deb_file, line_str))
    //     {
    //         double index = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double cost = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double err = std::stod(line_str);
    //         getline(deb_file, line_str);
    //         double mse = std::stod(line_str);

    //         costs.push_back(cost);
    //         mses.push_back(mse);
    //         errs.push_back(err);
    //     }

    //     int last_index = 0;

    //     for (int k = 0; k < ppm_costs.size(); ++k)
    //     {
    //         double eval = find_mse_for_cost(ppm_costs[k], last_index, costs, mses);
    //         deb_inf_mses[k] = deb_inf_mses[k] + 1.0 / double(actual_index + 1) * (eval - deb_inf_mses[k]);
    //         eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
    //         deb_inf_errs[k] = deb_inf_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_inf_errs[k]);
    //     }

    //     actual_index++;
    // }

    // // write the deb results
    // std::ofstream deb_inf_res;
    // deb_inf_res.open(loc + "avg_equal_cost_deb_cone_alpha_80.txt");
    // for (int i = 0; i < ppm_costs.size(); ++i)
    // {
    //     deb_inf_res << ppm_costs[i] << std::endl;
    //     deb_inf_res << deb_inf_mses[i] << std::endl;
    //     deb_inf_res << deb_inf_errs[i] << std::endl;
    // }
    // deb_inf_res.close();
    // // DEBIASED INFINITE //

    // std::cout << "TOTAL FAILED TESTS: " << failed_tests.size() << std::endl;
}

void err_run()
{
    int start_index = 0;
    int end_index = 512;

    std::vector<double> ppm_costs = std::vector<double>();
    std::vector<double> ppm_errs = std::vector<double>();
    std::vector<double> deb_errs = std::vector<double>();

    std::string loc = "/Users/corneria/Documents/Research/bias_research/ConvergenceRater/results/photon_deb_vs_ppm/r_0_125_many/";

    int actual_index = 0;
    std::vector<int> failed_tests = std::vector<int>();

    // parse and process the ppm results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        if (failed_test(loc, i))
        {
            failed_tests.push_back(i);
            std::cout << "run failed: " << i << std::endl;
            continue;
        }

        std::cout << "incorporating ppm results from run: " << i << std::endl;
        std::ifstream ppm_file;
        ppm_file.open(loc + "equal_cost_ppm_" + std::to_string(i) + ".txt");

        int cnt = 0;

        std::string line_str;
        while (getline(ppm_file, line_str))
        {
            double index = std::stod(line_str);
            getline(ppm_file, line_str);
            double cost = std::stod(line_str);
            getline(ppm_file, line_str);
            double err = std::stod(line_str);
            getline(ppm_file, line_str);
            double mse = std::stod(line_str);

            if (i == start_index)
            {
                ppm_costs.push_back(cost);
                ppm_errs.push_back(err);
            }
            else
            {
                ppm_costs[cnt] = ppm_costs[cnt] + 1.0 / double(actual_index + 1) * (cost - ppm_costs[cnt]);
                ppm_errs[cnt] = ppm_errs[cnt] + 1.0 / double(actual_index + 1) * (err - ppm_errs[cnt]);
                cnt++;
            }
        }

        actual_index++;
    }

    // write the ppm results
    std::ofstream ppm_res;
    ppm_res.open(loc + "avg_equal_cost_ppm_err.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        ppm_res << ppm_costs[i] << std::endl;
        ppm_res << ppm_errs[i] << std::endl;
        deb_errs.push_back(0.0);
    }
    ppm_res.close();

    actual_index = 0;

    // parse and process the deb results for index i
    for (int i = start_index; i < end_index; ++i)
    {
        for (int k = 0; k < failed_tests.size(); ++k)
        {
            if (failed_tests[k] == i)
                continue;
        }

        std::cout << "incorporating deb results from run: " << i << std::endl;
        std::ifstream deb_file;
        deb_file.open(loc + "equal_cost_deb_" + std::to_string(i) + ".txt");

        std::vector<double> costs = std::vector<double>();
        std::vector<double> errs = std::vector<double>();

        // int cnt = 0;

        std::string line_str;
        while (getline(deb_file, line_str))
        {
            double index = std::stod(line_str);
            getline(deb_file, line_str);
            double cost = std::stod(line_str);
            getline(deb_file, line_str);
            double err = std::stod(line_str);
            getline(deb_file, line_str);
            double mse = std::stod(line_str);

            costs.push_back(cost);
            errs.push_back(err);
        }

        // std::cout << "parsed the thing" << std::endl;
        int last_index = 0;

        for (int k = 0; k < ppm_costs.size(); ++k)
        {
            double eval = find_err_for_cost(ppm_costs[k], last_index, costs, errs);
            deb_errs[k] = deb_errs[k] + 1.0 / double(actual_index + 1) * (eval - deb_errs[k]);
        }

        actual_index++;
    }

    // write the deb results
    std::ofstream deb_res;
    deb_res.open(loc + "avg_equal_cost_deb_err.txt");
    for (int i = 0; i < ppm_costs.size(); ++i)
    {
        deb_res << ppm_costs[i] << std::endl;
        deb_res << deb_errs[i] << std::endl;
    }
    deb_res.close();

    std::cout << "TOTAL FAILED TESTS: " << failed_tests.size() << std::endl;
}

int main(int argc, char *argv[])
{
    // mse_run();
    // err_run();
    // opt_run();
    // alp_compare_run();
    final_run();

    // // dumb test
    // double val = 1.0;
    // bool running = true;

    // while (running)
    // {
    //     std::cout << "val: " << val << " works" << std::endl;
    //     val *= 2.0;
    //     double new_val = val + 1.0;
    //     if (new_val == val)
    //     {
    //         std::cout << "val: " << val << " is the last possible incrementable number!!" << std::endl;
    //         running = false;
    //     }
    //     // val *= 2.0;
    // }

    return 0;
}
