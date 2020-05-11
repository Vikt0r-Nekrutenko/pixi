#include "detector.hpp"
#include <ctime>

pixi::math::detector::detector(const pixi::math::dword &kw, const pixi::math::dword &kh, const pixi::math::dword &gw, const pixi::math::dword &gh, const pixi::math::dword iters)
    : _kohonen_layer(kw, kh, true, -1.f, +1.f),
      _grossberg_layer(gw, gh, true, -1.f, +1.f)
{
    _alpha_step = _alpha / (iters + 1);
    _beta_step  = _beta  / (iters + 1);
}

void pixi::math::detector::save(const char *file_name) const
{
    FILE *file = fopen(file_name, "wb");
    assert(file != nullptr);

    dword kw = _kohonen_layer.w(), kh = _kohonen_layer.h();
    dword gw = _grossberg_layer.w(), gh = _grossberg_layer.h();

    fwrite(&kw, 4, 1, file); fwrite(&kh, 4, 1, file);
    fwrite(&gw, 4, 1, file); fwrite(&gh, 4, 1, file);

    for (dword y = 0; y < _kohonen_layer.h(); y++) {
        for (dword x = 0; x < _kohonen_layer.w(); x++) {
            fwrite(&_kohonen_layer[y][x], 4, 1, file);
        }
    }
    for (dword y = 0; y < _grossberg_layer.h(); y++) {
        for (dword x = 0; x < _grossberg_layer.w(); x++) {
            fwrite(&_grossberg_layer[y][x], 4, 1, file);
        }
    }
    fclose(file);
    printf("saved!\n");
}

void pixi::math::detector::load(const char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    assert(file != nullptr);

    dword kw = _kohonen_layer.w(), kh = _kohonen_layer.h();
    dword gw = _grossberg_layer.w(), gh = _grossberg_layer.h();

    fread(&kw, 4, 1, file); fread(&kh, 4, 1, file);
    fread(&gw, 4, 1, file); fread(&gh, 4, 1, file);

    _kohonen_layer   = matrix(kw, kh);
    _grossberg_layer = matrix(gw, gh);

    for (dword y = 0; y < _kohonen_layer.h(); y++) {
        for (dword x = 0; x < _kohonen_layer.w(); x++) {
            fread(&_kohonen_layer[y][x], 4, 1, file);
        }
    }
    for (dword y = 0; y < _grossberg_layer.h(); y++) {
        for (dword x = 0; x < _grossberg_layer.w(); x++) {
            fread(&_grossberg_layer[y][x], 4, 1, file);
        }
    }

    fclose(file);
    printf("loaded!");
}

void pixi::math::detector::learn(const pixi::files::file &swds, const pixi::files::file &mwds, const pixi::math::dword &files_num, const vector<> swo, const vector<> mwo)
{
    time_t t1 = clock();
    for (size_t i = 0; i < files_num; i++)
    {
        if (_alpha < 0.001f || _beta < 0.001f) break;

        learn_step(swds.signature(i), swo);
        learn_step(mwds.signature(i), mwo);

    }
    printf("Elapsed: %f\tAlpha: %f\tBeta: %f\n", (clock() - t1) / 1000.f, _alpha, _beta);
}

void pixi::math::detector::learn_step(const vector<> &input, const vector<> &output)
{
    dword winner_indx = winner(input);
    vector<> &winner_neuron = _kohonen_layer[winner_indx];

    if ((int(output[0]) == -1 && winner_indx > _kohonen_layer.h() >> 1) || (int(output[0]) == +1 && winner_indx < _kohonen_layer.h() >> 1)) {
        winner_neuron = winner_neuron - (input - winner_neuron) * _alpha;
    } else {
        winner_neuron = winner_neuron + (input - winner_neuron) * _alpha;
    }

    for (dword i = 0; i < _grossberg_layer.h(); ++i) {
        float &vs = _grossberg_layer[i][winner_indx];
        vs = vs + (output[i] - vs) * _beta;
    }

    _alpha  -= _alpha_step;
    _beta   -= _beta_step;
}

pixi::math::vector<> pixi::math::detector::result(const vector<> &input)
{
    dword winner_indx = winner(input);
    vector<> result(_grossberg_layer.h());

    for (dword i = 0; i < _grossberg_layer.h(); i++) {
        result[i] = _grossberg_layer[i][winner_indx];
    }

    return result;
}

pixi::math::dword pixi::math::detector::statistic(const pixi::files::file &swds_file, const pixi::files::file &mwds_file, const pixi::math::dword &files_num, const bool output_is_enabled)
{
    dword sw_errs = 0u;
    for (size_t i = 0; i < files_num; i++)
    {
        vector<> sw = result(swds_file.signature(i));
        if(sw[0] > 0.f || sw[0] < -1.f) {
            sw_errs++;
        }
    }
    if (output_is_enabled) printf("Software errors: %d\n", sw_errs);

    dword mw_errs = 0u;
    for (size_t i = 0; i < files_num; i++)
    {
        vector<> mw = result(mwds_file.signature(i));
        if(mw[0] < 0.f || mw[0] > 1.f) {
            mw_errs++;
        }
    }
    if (output_is_enabled) printf("Malware errors:  %d\n", mw_errs);

    return sw_errs + mw_errs;
}

pixi::math::dword pixi::math::detector::winner(const vector<> &input) const
{
    vector<> weighted_sum = _kohonen_layer.dot(input);
    return weighted_sum.max();
}
