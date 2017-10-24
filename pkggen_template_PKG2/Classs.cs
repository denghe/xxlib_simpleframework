#pragma warning disable 0169, 0414
using TemplateLibrary;

[Desc("测试一下各种数据类型")]
class 基类
{
    bool 不淋;
    byte 白特;
    sbyte 撕白特;
    short 吸哦特;
    ushort 又吸哦特;
    int 硬特;
    uint 又硬特;
    long 浪;
    ulong 又浪;
    float 扶裸特;
    double 大波;
    string 湿最硬;
    BBuffer 屄拔扶儿;
}

class 派生类 : 基类
{
    List<byte> 立丝特白特;
    List<sbyte> 立丝特撕白特;
    List<short> 立丝特吸哦特;
    List<ushort> 立丝特又吸哦特;
    List<int> 立丝特硬特;
    List<uint> 立丝特又硬特;
    List<long> 立丝特浪;
    List<ulong> 立丝特又浪;
    List<float> 立丝特扶裸特;
    List<double> 立丝特大波;
    List<string> 立丝特湿最硬;
    List<BBuffer> 立丝特屄拔扶儿;
    [Desc("可以放入 基类 或 派生类")]
    List<基类> 立丝特基类;
}
