#ifndef __CONFIG_H__
#define __CONFIG_H__

namespace CFG
{
    const int seq_len = 128;
    const int head_num = 12;
    const int head_dim = 64;
    const int model_dim = 768;
    const int ff_dim = 3072;
    const float eps = 1e-5;

} // namespace CONFIG

#endif