using System.Diagnostics;

namespace 麻将
{
    public enum 牌 : byte
    {
        一筒 = 1,
        二筒,
        三筒,
        四筒,
        五筒,
        六筒,
        七筒,
        八筒,
        九筒,
        一条 = 11,
        二条,
        三条,
        四条,
        五条,
        六条,
        七条,
        八条,
        九条,
        一万 = 21,
        二万,
        三万,
        四万,
        五万,
        六万,
        七万,
        八万,
        九万,
        //红中 = 31,
        //发财 = 32,  // 33
        //白板 = 33,  // 35
    }

    public struct 牌张
    {
        public 牌 牌;
        public byte 张;
    }

    public class 手牌
    {
        public xx.List<牌> 牌s;

        public xx.List<牌张> 牌张s = new xx.List<牌张>();

        public void 填充牌张s()
        {
            Debug.Assert(牌s != null && 牌s.dataLen > 0);
            牌s.Sort();
            牌张s.Clear();
            var p = 牌s[0];
            byte c = 1;
            for (int i = 1; i < 牌s.dataLen; ++i)
            {
                if (牌s[i] == p)
                {
                    c++;
                }
                else
                {
                    牌张s.Add(new 牌张 { 牌 = p, 张 = c });
                    p = 牌s[i];
                    c = 1;
                }
            }
            牌张s.Add(new 牌张 { 牌 = p, 张 = c });
        }

        // len:1, count:2/3 代表拿掉 对子/刻子.   len:3, count:1 代表拿掉顺子
        public static xx.List<牌张> 拿掉牌(xx.List<牌张> 牌张s, int idx, int len, int count)
        {
            var ps = new xx.List<牌张>();
            var buf = 牌张s.buf;
            for (int i = 0; i < idx; ++i)
            {
                ps.Add(buf[i]);
            }
            for (int i = idx; i < idx + len; ++i)
            {
                var p = buf[i];
                Debug.Assert(p.张 >= count);
                if (p.张 > count)
                {
                    ps.Add(new 牌张 { 牌 = p.牌, 张 = (byte)(p.张 - count) });
                }
            }
            for (int i = idx + len; i < 牌张s.dataLen; ++i)
            {
                ps.Add(buf[i]);
            }
            return ps;
        }

        public bool 简单判断是否能胡()
        {
            for (int i = 0; i < 牌张s.dataLen; ++i)
            {
                if (牌张s[i].张 >= 2 && 简单判断是否能胡(拿掉牌(牌张s, i, 1, 2))) return true;
            }
            return false;
        }
        public static bool 简单判断是否能胡(xx.List<牌张> 牌张s)
        {
            if (牌张s.dataLen == 0) return true;
            // 拿掉刻
            for (int i = 0; i < 牌张s.dataLen; ++i)
            {
                if (牌张s[i].张 >= 3 && 简单判断是否能胡(拿掉牌(牌张s, i, 1, 3))) return true;
            }
            // 拿掉顺
            for (int i = 0; i < 牌张s.dataLen - 2; ++i)
            {
                if ((int)牌张s[i].牌 + 1 == (int)牌张s[i + 1].牌
                    && (int)牌张s[i].牌 + 2 == (int)牌张s[i + 2].牌
                    && 简单判断是否能胡(拿掉牌(牌张s, i, 3, 1))) return true;
            }
            return false;
        }
    }
}

public static class Program
{
    static void Main(string[] args)
    {
        var sp = new 麻将.手牌
        {
            //牌s = new xx.List<麻将.牌>(
            //    麻将.牌.二万, 麻将.牌.二万, 麻将.牌.二万,
            //    麻将.牌.一万, 麻将.牌.一万, 麻将.牌.一万,
            //    麻将.牌.五万, 麻将.牌.五万,
            //    麻将.牌.三万, 麻将.牌.三万, 麻将.牌.三万,
            //    麻将.牌.四万, 麻将.牌.四万, 麻将.牌.四万
            //)
            牌s = new xx.List<麻将.牌>(
                麻将.牌.一万, 麻将.牌.二万, 麻将.牌.三万,
                麻将.牌.一条, 麻将.牌.二条, 麻将.牌.三条,
                麻将.牌.五万, 麻将.牌.五万,
                麻将.牌.一筒, 麻将.牌.二筒, 麻将.牌.三筒,
                麻将.牌.四筒, 麻将.牌.二筒, 麻将.牌.三筒
            )
        };
        sp.填充牌张s();
        var sw = Stopwatch.StartNew();
        //int count = 0;
        for (int i = 0; i < 1000000; i++)
        {
            /*if (*/sp.简单判断是否能胡()/*) ++count*/;
        }
        System.Console.WriteLine("sw = " + sw.ElapsedMilliseconds);
    }
}

