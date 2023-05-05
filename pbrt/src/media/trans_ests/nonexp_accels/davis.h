#pragma once

#include "pbrt.h"

namespace pbrt
{

struct MicroDerivativeList
{
    MicroDerivativeList()
    {
        vals = std::vector<Float>();
    }

    void add_micro_derivative(Float val)
    {
        vals.push_back(val);
    }

    std::vector<Float> vals;
};

struct DavisMicroCellCoeff
{
    DavisMicroCellCoeff() : val(0.0), i(0.0), j(0.0) { }
    DavisMicroCellCoeff(Float val, Float i, Float j)
        : val(val), i(i), j(j) { }
    DavisMicroCellCoeff(const DavisMicroCellCoeff& other)
        : val(other.val), i(other.i), j(other.j) { }

    bool matches(Float ii, Float jj)
    {
        return i == ii && j == jj;
    }

    // i is the power of the x, j is the power of the (cx^b +1)
    Float val;
    Float i;
    Float j;
};

struct DavisMicroCoeffGrid
{
    DavisMicroCoeffGrid()
    {
        cells_one = std::vector<DavisMicroCellCoeff>();
        cells_two = std::vector<DavisMicroCellCoeff>();
        is_one = true;

        first_log_term_val = 0.0;
        first_log_term_i = 0.0;
    }

    Float compute_contrib() const
    {
        Float contrib = first_log_term_val;

        if (is_one)
        {
            if (cells_one.size() == 0) return 1.0;
            for (int i = 0; i < cells_one.size(); ++i)
            {
                contrib += cells_one[i].val;
            }
        }
        else
        {
            if (cells_two.size() == 0) return 1.0;
            for (int i = 0; i < cells_two.size(); ++i)
            {
                contrib += cells_two[i].val;
            }
        }

        return contrib;
    }

    void start_new_iter()
    {
        if (is_one)
        {
            cells_two.clear();
        }
        else
        {
            cells_one.clear();
        }
    }

    void add_contrib(Float val, Float i, Float j)
    {
        bool found = false;

        if (is_one)
        {
            for (int k = 0; k < cells_two.size(); ++k)
            {
                if (cells_two[k].matches(i, j))
                {
                    cells_two[k].val += val;
                    found = true;
                    k = cells_two.size();
                }
            }

            if (!found)
            {
                cells_two.push_back(DavisMicroCellCoeff(val, i, j));
            }
        }
        else
        {
            for (int k = 0; k < cells_one.size(); ++k)
            {
                if (cells_one[k].matches(i, j))
                {
                    cells_one[k].val += val;
                    found = true;
                    k = cells_one.size();
                }
            }

            if (!found)
            {
                cells_one.push_back(DavisMicroCellCoeff(val, i, j));
            }
        }
    }

    Float compute_beta_c(Float c, Float beta)
    {
        return pow(c, beta + 1.0);
    }

    Float compute_log_coeff(Float pivot, Float c, Float beta)
    {
        return c*pow(pivot, beta) + 1.0;
    }

    Float compute_log(Float pivot, Float c, Float beta)
    {
        return std::log(compute_log_coeff(pivot, c, beta));
    }

    // NOTE: this has to be called after compute contrib
    void update_contrib(int deriv, Float pivot, Float pre_c, Float beta)
    {
        Float c = compute_beta_c(pre_c, beta);
        Float log_coeff = compute_log_coeff(pivot, c, beta);
        Float log_val = compute_log(pivot, c, beta);

        if (deriv == 1) // special procedure to initialize the first derivative
        {
            assert(!cells_two.size()); // there must not exist any coefficients

            first_log_term_val = (1.0 - beta) * log_val / (c * pow(pivot, beta));
            first_log_term_i = -beta;

            Float val_1 = beta / log_coeff;
            Float i_1 = 0.0;
            Float j_1 = 1.0;

            // std::cout << "pre contrib: " << std::endl;
            add_contrib(val_1, i_1, j_1);
        }
        else
        {
            // compute the derivatives of the first term
            Float val_0 = first_log_term_val * pow(pivot, beta - 1.0) * beta * c
                                              / (log_val * log_coeff);
            Float i_0 = first_log_term_i + beta - 1.0;
            Float j_0 = 1.0;

            first_log_term_val = first_log_term_val * (first_log_term_i / pivot);
            first_log_term_i -= 1.0;

            // std::cout << "ack three: " << val_0 << std::endl;

            add_contrib(val_0, i_0, j_0);

            if (is_one)
            {
                for (int k = 0; k < cells_one.size(); ++k)
                {
                    // compute two derivatives
                    if (cells_one[k].i != 0.0)
                    {
                        Float val_1 = cells_one[k].val * (cells_one[k].i) / pivot;
                        Float i_1 = cells_one[k].i - 1.0;
                        Float j_1 = cells_one[k].j;

                        add_contrib(val_1, i_1, j_1);
                    }

                    Float val_2 = cells_one[k].val * -cells_one[k].j * beta *
                                   pow(pivot, beta - 1.0) * c / log_coeff;
                    Float i_2 = cells_one[k].i + beta - 1.0;
                    Float j_2 = cells_one[k].j + 1.0;

                    add_contrib(val_2, i_2, j_2);
                }
            }
            else
            {
                for (int k = 0; k < cells_two.size(); ++k)
                {
                    // compute two derivatives
                    if (cells_two[k].i != 0.0)
                    {
                        Float val_1 = cells_two[k].val * (cells_two[k].i) / pivot;
                        Float i_1 = cells_two[k].i - 1.0;
                        Float j_1 = cells_two[k].j;

                        add_contrib(val_1, i_1, j_1);
                    }

                    Float val_2 = cells_two[k].val * -cells_two[k].j * beta *
                                   pow(pivot, beta - 1.0) * c / log_coeff;
                    Float i_2 = cells_two[k].i + beta - 1.0;
                    Float j_2 = cells_two[k].j + 1.0;

                    add_contrib(val_2, i_2, j_2);
                }
            }
        }
    }

    void finalize_contrib()
    {
        is_one = !is_one;
    }

    Float first_log_term_val;
    Float first_log_term_i;

    std::vector<DavisMicroCellCoeff> cells_one;
    std::vector<DavisMicroCellCoeff> cells_two;
    bool is_one;
};

// TODO: create the micro grid and test out its derivatives vs. the two known ones

struct DavisMacroCellCoeffElement
{
    DavisMacroCellCoeffElement() : i(0), j(0) { }
    DavisMacroCellCoeffElement(int i, int j)
        : i(i), j(j) { }
    DavisMacroCellCoeffElement(const DavisMacroCellCoeffElement& other)
        : i(other.i), j(other.j) { }

    bool matches(int ii, int jj) const
    {
        return i == ii && j == jj;
    }

    bool matches(const DavisMacroCellCoeffElement& other) const
    {
        return i == other.i && j == other.j;
    }

    void print_this() const
    {
        std::cout << "f(" << i << ", " << j << ")";
    }

    // i is the derivative, j is the power
    int i; int j;
};

struct DavisMacroCellCoeff
{
    DavisMacroCellCoeff() : val(0.0)
    {
        elements = std::vector<DavisMacroCellCoeffElement>();
    }

    DavisMacroCellCoeff(const DavisMacroCellCoeff& other)
        : val(other.val)
    {
        elements = std::vector<DavisMacroCellCoeffElement>();

        for (int i = 0; i < other.elements.size(); ++i)
        {
            elements.push_back(other.elements[i]);
        }
    }

    DavisMacroCellCoeff(DavisMacroCellCoeffElement element) : val(0.0)
    {
        elements = std::vector<DavisMacroCellCoeffElement>();
        elements.push_back(element);
    }

    DavisMacroCellCoeff(const std::vector<DavisMacroCellCoeffElement>& elems) : val(0.0)
    {
        elements = std::vector<DavisMacroCellCoeffElement>();

        for (int i = 0; i < elems.size(); ++i)
        {
            elements.push_back(elems[i]);
        }

        val = 0.0;
    }

    void addElement(int i, int j)
    {
        if (i == 0 || j == 0) return;

        bool match = false;
        for (int k = 0; k < elements.size(); ++k)
        {
            if (elements[k].i == i)
            {
                match = true;
                elements[k].j++;
            }
        }

        if (!match)
        {
            elements.push_back(DavisMacroCellCoeffElement(i, j));
        }
    }

    void addElement(const DavisMacroCellCoeffElement& other)
    {
        bool found = false;
        for (int k = 0; k < elements.size(); ++k)
        {
            if (elements[k].i == other.i)
            {
                found = true;
                elements[k].j += other.j;
            }
        }

        if (!found)
        {
            elements.push_back(other);
        }
    }

    bool matches(const std::vector<DavisMacroCellCoeffElement>& other) const
    {
        if (other.size() != elements.size()) return false;

        // this is inefficient but what the heck, idc at this point
        for (int i = 0; i < elements.size(); ++i)
        {
            bool match = false;

            for (int j = 0; j < other.size(); ++j)
            {
                if (other[j].matches(elements[i]))
                {
                    match = true;
                    j = other.size();
                }
            }

            if (!match) return false;
        }

        return true;
    }

    // for debugging purposes
    void print_this() const
    {
        std::cout << val;
        for (int i = 0; i < elements.size(); ++i)
        {
            elements[i].print_this();
        }
    }

    std::vector<DavisMacroCellCoeffElement> elements;
    Float val;
};

struct DavisMacroGrid
{
    DavisMacroGrid()
    {
        cells_one = std::vector<DavisMacroCellCoeff>();
        cells_two = std::vector<DavisMacroCellCoeff>();
        is_one = true;
    }

    Float compute_contrib()
    {
        Float contrib = 0.0;

        // std::cout << "computing contrib" << std::endl;

        if (is_one)
        {
            if (cells_one.size() == 0) return 1.0;
            for (int i = 0; i < cells_one.size(); ++i)
            {
                contrib += cells_one[i].val;
                // std::cout << "i: " << i << " val: " << cells_one[i].val << std::endl;
            }
        }
        else
        {
            if (cells_two.size() == 0) return 1.0;
            for (int i = 0; i < cells_two.size(); ++i)
            {
                contrib += cells_two[i].val;
                // std::cout << "i: " << i << " val: " << cells_two[i].val << std::endl;
            }
        }

        // std::cout << std::endl;

        return contrib;
    }

    void start_new_iter()
    {
        if (is_one)
        {
            cells_two.clear();
        }
        else
        {
            cells_one.clear();
        }
    }

    void add_contrib(Float val, const std::vector<DavisMacroCellCoeffElement>& cof)
    {
        bool found = false;

        if (is_one)
        {
            for (int k = 0; k < cells_two.size(); ++k)
            {
                if (cells_two[k].matches(cof))
                {
                    cells_two[k].val += val;
                    found = true;
                    k = cells_two.size();
                }
            }

            if (!found)
            {
                DavisMacroCellCoeff coffee = DavisMacroCellCoeff(cof);
                coffee.val = val;
                cells_two.push_back(coffee);
            }
        }
        else
        {
            for (int k = 0; k < cells_one.size(); ++k)
            {
                if (cells_one[k].matches(cof))
                {
                    cells_one[k].val += val;
                    found = true;
                    k = cells_one.size();
                }
            }

            if (!found)
            {
                DavisMacroCellCoeff coffee = DavisMacroCellCoeff(cof);
                coffee.val = val;
                cells_one.push_back(coffee);
            }
        }
    }

    void add_contrib(DavisMacroCellCoeff elem)
    {
        bool found = false;

        if (is_one)
        {
            for (int k = 0; k < cells_two.size(); ++k)
            {
                if (cells_two[k].matches(elem.elements))
                {
                    cells_two[k].val += elem.val;
                    found = true;
                    k = cells_two.size();
                }
            }

            if (!found)
            {
                cells_two.push_back(elem);
            }
        }
        else
        {
            for (int k = 0; k < cells_one.size(); ++k)
            {
                if (cells_one[k].matches(elem.elements))
                {
                    cells_one[k].val += elem.val;
                    found = true;
                    k = cells_one.size();
                }
            }

            if (!found)
            {
                cells_one.push_back(elem);
            }
        }
    }

    void update_contrib(const MicroDerivativeList& derivs)
    {
        // std::cout << "booya: " << cells_one.size() << std::endl;
        // std::cout << derivs.vals[1] << std::endl;
        if (is_one)
        {
            for (int k = 0; k < cells_one.size(); ++k)
            {
                // std::cout << "huh" << std::endl;
                Float val_1 = cells_one[k].val * -derivs.vals[1];

                // std::cout << cells_one.size() << std::endl;

                // std::cout << "val_1: " << val_1 << std::endl;

                // std::vector<DavisMacroCellCoeffElement> new_elems_1 =
                //     std::vector<DavisMacroCellCoeffElement>();

                DavisMacroCellCoeff one = cells_one[k];
                one.addElement(1, 1);
                one.val = val_1;
                add_contrib(one);
                // cells_two.push_back(one);

                for (int kk = 0; kk < cells_one[k].elements.size(); ++kk)
                {
                    DavisMacroCellCoeffElement deriv_element = cells_one[k].elements[kk];
                    int power = deriv_element.j;
                    int deriv = deriv_element.i;
                    // std::cout << "deriv_elem: " << deriv << std::endl;

                    DavisMacroCellCoeff new_one = DavisMacroCellCoeff();
                    Float val_new = cells_one[k].val;

                    val_new *= Float(power) * (derivs.vals[deriv+1] / derivs.vals[deriv]);

                    new_one.addElement(deriv, power - 1);
                    new_one.addElement(deriv+1, 1);

                    for (int jk = 0; jk < cells_one[k].elements.size(); ++jk)
                    {
                        if (jk != kk)
                        {
                            new_one.addElement(cells_one[k].elements[jk]);
                        }
                    }

                    new_one.val = val_new;

                    add_contrib(new_one);

                    // cells_two.push_back(new_one);
                }
            }

            if (cells_one.size() == 0)
            {
                DavisMacroCellCoeff one = DavisMacroCellCoeff();
                one.addElement(1, 1);
                one.val = -derivs.vals[1];
                add_contrib(one);
            }
        }
        else
        {
            for (int k = 0; k < cells_two.size(); ++k)
            {
                Float val_1 = cells_two[k].val * -derivs.vals[1];

                // std::cout << cells_two.size() << std::endl;

                // std::cout << "val_1: " << val_1 << std::endl;

                DavisMacroCellCoeff one = cells_two[k];
                one.addElement(1, 1);
                one.val = val_1;
                add_contrib(one);

                for (int kk = 0; kk < cells_two[k].elements.size(); ++kk)
                {
                    DavisMacroCellCoeffElement deriv_element = cells_two[k].elements[kk];
                    int power = deriv_element.j;
                    int deriv = deriv_element.i;

                    DavisMacroCellCoeff new_one = DavisMacroCellCoeff();
                    Float val_new = cells_two[k].val;

                    val_new *= Float(power) * (derivs.vals[deriv+1] / derivs.vals[deriv]);

                    new_one.addElement(deriv, power - 1);
                    new_one.addElement(deriv + 1, 1);

                    for (int jk = 0; jk < cells_two[k].elements.size(); ++jk)
                    {
                        if (jk != kk)
                        {
                            new_one.addElement(cells_two[k].elements[jk]);
                        }
                    }

                    new_one.val = val_new;

                    add_contrib(new_one);
                }
            }
            if (cells_two.size() == 0)
            {
                assert(false);
                DavisMacroCellCoeff one = DavisMacroCellCoeff();
                one.addElement(1, 1);
                one.val = -derivs.vals[1];
                add_contrib(one);
            }
        }
    }

    // this is for initialization
    void add_element(const DavisMacroCellCoeff& other)
    {
        cells_one.push_back(other);
        cells_two.push_back(other);
    }

    void finalize_contrib()
    {
        is_one = !is_one;
    }

    // for debugging purposes
    void print_this() const
    {
        if (is_one)
        {
            for (int i = 0; i < cells_one.size(); ++i)
            {
                cells_one[i].print_this();
                if (i != cells_one.size() - 1)
                {
                    std::cout << " + ";
                }
            }
            std::cout << std::endl;
        }
        else
        {
            for (int i = 0; i < cells_two.size(); ++i)
            {
                cells_two[i].print_this();
                if (i != cells_two.size() - 1)
                {
                    std::cout << " + ";
                }
            }
            std::cout << std::endl;
        }
    }

    std::vector<DavisMacroCellCoeff> cells_one;
    std::vector<DavisMacroCellCoeff> cells_two;
    bool is_one;
};

}
