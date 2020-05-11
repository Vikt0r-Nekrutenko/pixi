#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "matrix.hpp"
#include "file.hpp"

namespace pixi { namespace math {

class KERNEL_EXPORT detector
{
public:
    detector(const dword &kw, const dword &kh, const dword &gw, const dword &gh, const dword iters);
    inline float alpha() const
    {
        return _alpha;
    }
    inline float beta() const
    {
        return _beta;
    }
    void save(const char *file_name) const;
    void load(const char *file_name);
    void learn(const files::file &swds, const files::file &mwds, const dword &files_num, const vector<> swo, const vector<> mwo);
    void learn_step(const vector<> &input, const vector<> &output);
    vector<> result(const vector<> &input);
    dword statistic(const files::file &swds_file, const files::file &mwds_file, const dword &files_num, const bool output_is_enabled = false);
    dword winner(const vector<> &input) const;
private:
    matrix _kohonen_layer;
    matrix _grossberg_layer;

    float _alpha      = 0.7f, _beta      = 0.1f;
    float _alpha_step = 0.0f, _beta_step = 0.0f;
};

}}

#endif // DETECTOR_HPP
