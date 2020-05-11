#include <iostream>
#include "file.hpp"
#include "matrix.hpp"
#include <cassert>
#include <cmath>
#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>


namespace pixi { namespace math {

class detector
{
public:
    detector(const dword &kw, const dword &kh, const dword &gw, const dword &gh, const dword iters)
        : _kohonen_layer(kw, kh, true, -1.f, +1.f),
          _grossberg_layer(gw, gh, true, -1.f, +1.f)
    {
        _alpha_step = _alpha / (iters + 1);
        _beta_step  = _beta  / (iters + 1);
    }
    void save(const char *file_name) const
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
    void load(const char *file_name)
    {
        FILE *file = fopen(file_name, "rb");
        assert(file != nullptr);

        dword kw = _kohonen_layer.w(), kh = _kohonen_layer.h();
        dword gw = _grossberg_layer.w(), gh = _grossberg_layer.h();

        fread(&kw, 4, 1, file); fread(&kh, 4, 1, file);
        fread(&gw, 4, 1, file); fread(&gh, 4, 1, file);

        _kohonen_layer   = matrix<>(kw, kh);
        _grossberg_layer = matrix<>(gw, gh);

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
    void learn(const files::file &swds, const files::file &mwds, const dword &files_num, const vector<> swo, const vector<> mwo)
    {
        time_t t1 = clock();
        while (true)
        {
            for (size_t i = 0; i < files_num; i++)
            {
                if (_alpha < 0.001f || _beta < 0.001f) goto m;

                learn_step(swds.signature(i), swo);
                learn_step(mwds.signature(i), mwo);
            }
            printf("Elapsed: %f\tAlpha: %f\tBeta: %f\r", (clock() - t1) / 1000.f, _alpha, _beta);
        }
        m: printf("Elapsed: %f\tAlpha: %f\tBeta: %f\n", (clock() - t1) / 1000.f, _alpha, _beta);

    }
    void learn_step(const vector<> &input, const vector<> &output)
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
    vector<> result(const vector<> &input)
    {
        dword winner_indx = winner(input);
        vector<> result(_grossberg_layer.h());

        for (dword i = 0; i < _grossberg_layer.h(); i++) {
            result[i] = _grossberg_layer[i][winner_indx];
        }

        return result;
    }
    dword statistic(const files::file &swds_file, const files::file &mwds_file, const dword &files_num, const bool output_is_enabled = false)
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
    dword winner(const vector<> &input) const
    {
         vector<> weighted_sum = _kohonen_layer.dot(input);
         return weighted_sum.max();
    }
private:
    matrix<> _kohonen_layer;
    matrix<> _grossberg_layer;

    float _alpha      = 0.7f, _beta      = 0.1f;
    float _alpha_step = 0.0f, _beta_step = 0.0f;
};

}}

using namespace pixi::math;

dword random(dword min, dword max) {
    return min + rand() % (max - min);
}

int main()
{
//    std::srand(std::time(nullptr));
    pixi::files::file swds("../Kernel/data/SoftwareDataSetRaw.data");
//    pixi::files::file swds("../Kernel/data/SoftwareDataSet.data");
    pixi::files::file mwds("../Kernel/data/MalwareDataSet.data");
//    pixi::files::file swds("../../Kernel/data/SoftwareDataSetRaw.data");
//    pixi::files::file mwds("../../Kernel/data/MalwareDataSet.data");

    dword sixteen = 17;
    dword files_num = sixteen;//469 * 40;

    std::atomic<dword> minerr(0xffffffff);

    detector d1(sixteen, 4, 4, 2, 400);
    d1.learn(swds, mwds, files_num, vector<>(sixteen, -1.f), vector<>(sixteen, +1.f));
//    d1.load("63(2).txt");

    std::mutex mtx;
    auto www = [&](const dword min_it, const dword max_it) {
        int c = 100;
        while (c--)
        {
            dword lw = random(2, 16);
            dword li = random(min_it, max_it);

            detector det1(lw, 16, 2, lw, li);
            det1.learn(swds, mwds, files_num, vector<>(16, -1.f), vector<>(16, +1.f));

            dword err = det1.statistic(swds, mwds, files_num);

            if (err < minerr) {
                minerr = err;
                std::cout << "NNID" << std::this_thread::get_id() << ": " << minerr << std::endl
                          << "Kohonen layer: " << lw << " Iterations: " << li << std::endl;
                mtx.lock();
                d1 = det1;
                mtx.unlock();
            }
        }
        std::cout << "NNID" << std::this_thread::get_id() << ": work done!" << std::endl;
    };

//    std::thread t1(www, 100, 15000), t2(www, 15000, 30000);
//    std::thread t3(www, 10000, 15000), t4(www, 15000, 20000);
//    t1.join(); t2.join();
//    t3.join(); t4.join();

    std::cout << "END!" << std::endl;

    d1.statistic(swds, mwds, files_num, true);

    char v[64];
    while (v[0] != 'q')
    {
        std::cin >> v;
             if (v[0] == 'w') d1.result(swds.signature(random(0, files_num))).print();
        else if (v[0] == 'e') d1.result(mwds.signature(random(0, files_num))).print();
    }
    return 0;
}
