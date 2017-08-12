using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QiPai
{
    class ProgramTest
    {
        static void Main(string[] args)
        {

            //胡牌测试

            List<int> testList = new List<int>(14) { 1, 2,3,11,12,13,5,5,21,22,23,24,22,23 };
            testList.Sort();
            int testCount = 1000000;
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            for (int i=0;i< testCount;i++)
            {
                GetHuByCheckCard(testList);
            }
            
            sw.Stop();
            Console.WriteLine( testCount +"  次处理消耗时间:" + sw.ElapsedMilliseconds + "毫秒" + "\n\n\n");



            string GameValue = Console.ReadLine();//一次输入一行
        }




        /// <summary>
        /// 创建所有麻将胡牌类型
        /// </summary>
        /// <param name="MaJiangTypeCount"></param>
        private static void CreateMaJiangHuPaiList()
        {
            int MaJiangTypeCount = 37;

            List<int> MaJiangList = new List<int>();
            List<int> _maJiangTypeValueBase = new List<int>();
            if (MaJiangTypeCount < 30)
            {
                return;
            }
            //1 - 9代表万，11到19代表条，21到29代表筒，10，20，30作为界限，31—37（东南西北中发白）以后代表风向
            for (int j = 1; j <= MaJiangTypeCount; j++)
            {
                if (j % 10 != 0)//排除麻将类型界限值
                {
                    _maJiangTypeValueBase.Add(j);
                }
            }


            for (int j = 0; j < _maJiangTypeValueBase.Count; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    MaJiangList.Add(_maJiangTypeValueBase[j]);
                }
            }

            Dictionary<string, int> HuTypeList = new Dictionary<string, int>();

            for (int a = 0; a < MaJiangList.Count - 13; a++)
            {
                List<int> chechLista = new List<int>();
                chechLista.Add(MaJiangList[a]);
                for (int b = a + 1; b < MaJiangList.Count - 12; b++)
                {
                    List<int> chechListb = new List<int>(chechLista);
                    chechListb.Add(MaJiangList[b]);
                    for (int c = b + 1; c < MaJiangList.Count - 11; c++)
                    {
                        List<int> chechListc = new List<int>(chechListb);
                        chechListc.Add(MaJiangList[b]);
                        for (int d = c + 1; d < MaJiangList.Count - 10; d++)
                        {
                            List<int> chechListd = new List<int>(chechListc);
                            chechListd.Add(MaJiangList[d]);
                            for (int e = d + 1; e < MaJiangList.Count - 9; e++)
                            {
                                List<int> chechListe = new List<int>(chechListd);
                                chechListe.Add(MaJiangList[e]);
                                for (int f = e + 1; f < MaJiangList.Count - 8; f++)
                                {
                                    List<int> chechListf = new List<int>(chechListe);
                                    chechListf.Add(MaJiangList[f]);
                                    for (int g = f + 1; g < MaJiangList.Count - 7; g++)
                                    {
                                        List<int> chechListg = new List<int>(chechListf);
                                        chechListg.Add(MaJiangList[g]);
                                        for (int h = g + 1; h < MaJiangList.Count - 6; h++)
                                        {
                                            List<int> chechListh = new List<int>(chechListg);
                                            chechListh.Add(MaJiangList[h]);
                                            for (int i = h + 1; i < MaJiangList.Count - 5; i++)
                                            {
                                                List<int> chechListi = new List<int>(chechListh);
                                                chechListi.Add(MaJiangList[i]);
                                                for (int j = i + 1; j < MaJiangList.Count - 4; j++)
                                                {
                                                    List<int> chechListj = new List<int>(chechListi);
                                                    chechListj.Add(MaJiangList[j]);
                                                    for (int k = j + 1; k < MaJiangList.Count - 3; k++)
                                                    {
                                                        List<int> chechListk = new List<int>(chechListj);
                                                        chechListk.Add(MaJiangList[k]);
                                                        for (int l = k + 1; l < MaJiangList.Count - 2; l++)
                                                        {
                                                            List<int> chechListl = new List<int>(chechListk);
                                                            chechListl.Add(MaJiangList[l]);
                                                            for (int m = l + 1; m < MaJiangList.Count - 1; m++)
                                                            {
                                                                List<int> chechListm = new List<int>(chechListl);
                                                                chechListm.Add(MaJiangList[m]);
                                                                for (int n = m + 1; n < MaJiangList.Count; n++)
                                                                {
                                                                    List<int> chechListn = new List<int>(chechListm);
                                                                    chechListn.Add(MaJiangList[n]);
                                                                    if (GetHuByCheckCard(chechListn))
                                                                    {
                                                                        string s = "";
                                                                        foreach (int addValue in chechListn)
                                                                        {
                                                                            if (s == "")
                                                                            {
                                                                                s = addValue.ToString();
                                                                            }
                                                                            else
                                                                            {
                                                                                s += "," + addValue.ToString();
                                                                            }

                                                                        }
                                                                        if (!HuTypeList.ContainsKey(s))
                                                                        {
                                                                            HuTypeList.Add(s, 0);
                                                                        }

                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                    }
                }
            }

            for (int d = 0; d < MaJiangList.Count - 10; d++)
            {
                List<int> chechListd = new List<int>();
                chechListd.Add(MaJiangList[d]);
                for (int e = d + 1; e < MaJiangList.Count - 9; e++)
                {
                    List<int> chechListe = new List<int>(chechListd);
                    chechListe.Add(MaJiangList[e]);
                    for (int f = e + 1; f < MaJiangList.Count - 8; f++)
                    {
                        List<int> chechListf = new List<int>(chechListe);
                        chechListf.Add(MaJiangList[f]);
                        for (int g = f + 1; g < MaJiangList.Count - 7; g++)
                        {
                            List<int> chechListg = new List<int>(chechListf);
                            chechListg.Add(MaJiangList[g]);
                            for (int h = g + 1; h < MaJiangList.Count - 6; h++)
                            {
                                List<int> chechListh = new List<int>(chechListg);
                                chechListh.Add(MaJiangList[h]);
                                for (int i = h + 1; i < MaJiangList.Count - 5; i++)
                                {
                                    List<int> chechListi = new List<int>(chechListh);
                                    chechListi.Add(MaJiangList[i]);
                                    for (int j = i + 1; j < MaJiangList.Count - 4; j++)
                                    {
                                        List<int> chechListj = new List<int>(chechListi);
                                        chechListj.Add(MaJiangList[j]);
                                        for (int k = j + 1; k < MaJiangList.Count - 3; k++)
                                        {
                                            List<int> chechListk = new List<int>(chechListj);
                                            chechListk.Add(MaJiangList[k]);
                                            for (int l = k + 1; l < MaJiangList.Count - 2; l++)
                                            {
                                                List<int> chechListl = new List<int>(chechListk);
                                                chechListl.Add(MaJiangList[l]);
                                                for (int m = l + 1; m < MaJiangList.Count - 1; m++)
                                                {
                                                    List<int> chechListm = new List<int>(chechListl);
                                                    chechListm.Add(MaJiangList[m]);
                                                    for (int n = m + 1; n < MaJiangList.Count; n++)
                                                    {
                                                        List<int> chechListn = new List<int>(chechListm);
                                                        chechListn.Add(MaJiangList[n]);
                                                        if (GetHuByCheckCard(chechListn))
                                                        {
                                                            string s = "";
                                                            foreach (int addValue in chechListn)
                                                            {
                                                                if (s == "")
                                                                {
                                                                    s = addValue.ToString();
                                                                }
                                                                else
                                                                {
                                                                    s += "," + addValue.ToString();
                                                                }

                                                            }
                                                            if (!HuTypeList.ContainsKey(s))
                                                            {
                                                                HuTypeList.Add(s, 0);
                                                            }

                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int g = 0; g < MaJiangList.Count - 7; g++)
            {
                List<int> chechListg = new List<int>();
                chechListg.Add(MaJiangList[g]);
                for (int h = g + 1; h < MaJiangList.Count - 6; h++)
                {
                    List<int> chechListh = new List<int>(chechListg);
                    chechListh.Add(MaJiangList[h]);
                    for (int i = h + 1; i < MaJiangList.Count - 5; i++)
                    {
                        List<int> chechListi = new List<int>(chechListh);
                        chechListi.Add(MaJiangList[i]);
                        for (int j = i + 1; j < MaJiangList.Count - 4; j++)
                        {
                            List<int> chechListj = new List<int>(chechListi);
                            chechListj.Add(MaJiangList[j]);
                            for (int k = j + 1; k < MaJiangList.Count - 3; k++)
                            {
                                List<int> chechListk = new List<int>(chechListj);
                                chechListk.Add(MaJiangList[k]);
                                for (int l = k + 1; l < MaJiangList.Count - 2; l++)
                                {
                                    List<int> chechListl = new List<int>(chechListk);
                                    chechListl.Add(MaJiangList[l]);
                                    for (int m = l + 1; m < MaJiangList.Count - 1; m++)
                                    {
                                        List<int> chechListm = new List<int>(chechListl);
                                        chechListm.Add(MaJiangList[m]);
                                        for (int n = m + 1; n < MaJiangList.Count; n++)
                                        {
                                            List<int> chechListn = new List<int>(chechListm);
                                            chechListn.Add(MaJiangList[n]);
                                            if (GetHuByCheckCard(chechListn))
                                            {
                                                string s = "";
                                                foreach (int addValue in chechListn)
                                                {
                                                    if (s == "")
                                                    {
                                                        s = addValue.ToString();
                                                    }
                                                    else
                                                    {
                                                        s += "," + addValue.ToString();
                                                    }

                                                }
                                                if (!HuTypeList.ContainsKey(s))
                                                {
                                                    HuTypeList.Add(s, 0);
                                                }

                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int j = 0; j < MaJiangList.Count - 4; j++)
            {
                List<int> chechListj = new List<int>();
                chechListj.Add(MaJiangList[j]);
                for (int k = j + 1; k < MaJiangList.Count - 3; k++)
                {
                    List<int> chechListk = new List<int>(chechListj);
                    chechListk.Add(MaJiangList[k]);
                    for (int l = k + 1; l < MaJiangList.Count - 2; l++)
                    {
                        List<int> chechListl = new List<int>(chechListk);
                        chechListl.Add(MaJiangList[l]);
                        for (int m = l + 1; m < MaJiangList.Count - 1; m++)
                        {
                            List<int> chechListm = new List<int>(chechListl);
                            chechListm.Add(MaJiangList[m]);
                            for (int n = m + 1; n < MaJiangList.Count; n++)
                            {
                                List<int> chechListn = new List<int>(chechListm);
                                chechListn.Add(MaJiangList[n]);
                                if (GetHuByCheckCard(chechListn))
                                {
                                    string s = "";
                                    foreach (int addValue in chechListn)
                                    {
                                        if (s == "")
                                        {
                                            s = addValue.ToString();
                                        }
                                        else
                                        {
                                            s += "," + addValue.ToString();
                                        }

                                    }
                                    if (!HuTypeList.ContainsKey(s))
                                    {
                                        HuTypeList.Add(s, 0);
                                    }

                                }
                            }
                        }
                    }
                }
            }

            for (int m = 0; m < MaJiangList.Count - 1; m++)
            {
                List<int> chechListm = new List<int>();
                chechListm.Add(MaJiangList[m]);
                for (int n = m + 1; n < MaJiangList.Count; n++)
                {
                    List<int> chechListn = new List<int>(chechListm);
                    chechListn.Add(MaJiangList[n]);
                    if (GetHuByCheckCard(chechListn))
                    {
                        string s = "";
                        foreach (int addValue in chechListn)
                        {
                            if (s == "")
                            {
                                s = addValue.ToString();
                            }
                            else
                            {
                                s += "," + addValue.ToString();
                            }

                        }
                        if (!HuTypeList.ContainsKey(s))
                        {
                            HuTypeList.Add(s, 0);
                        }

                    }
                }
            }

            int count = HuTypeList.Count;
        }

        /// <summary>
        /// 检测当前手牌根据获得的牌是否能胡牌
        /// </summary>
        /// <param name="checkHandCardList">当前手牌</param>
        /// <param name="cardValue">所要检测的牌</param>
        /// <returns></returns>
        public static bool GetHuByCheckCard(List<int> checkHandCardList)
        {
            List<int> handCards = new List<int>(checkHandCardList);
            int length = handCards.Count;
            //获取对子信息
            List<int> duiZiList = new List<int>();
            for (int i = 0; i < length - 1; i++)
            {
                if (handCards[i] == handCards[i + 1])//有对子存在(四暗刻算2个对子)
                {
                    duiZiList.Add(handCards[i]);
                    i++;

                }
            }

            //没有对子不能胡牌
            if (duiZiList.Count == 0)
            {
                ////Server.Show("此牌没有对子信息");
                return false;
            }

            //是否为七对
            if (duiZiList.Count == 7)
            {
                return true;
            }


            //移除对子，开始判断牌型
            length = duiZiList.Count;
            if (duiZiList.Count == 1)//一个对子的时候
            {
                handCards.Remove(duiZiList[0]);
                handCards.Remove(duiZiList[0]);
                return ChecHandCardHu(handCards);
            }
            else
            {
                for (int i = 0; i < length; i++)
                {
                    if (i == (length - 1))//检测最后一个对子的时候
                    {
                        handCards.Remove(duiZiList[length - 1]);
                        handCards.Remove(duiZiList[length - 1]);
                        return ChecHandCardHu(handCards);
                    }
                    else
                    {
                        List<int> checkList = new List<int>(handCards);
                        //移除对子
                        checkList.Remove(duiZiList[i]);
                        checkList.Remove(duiZiList[i]);
                        if (ChecHandCardHu(checkList))
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        static bool falg = true;
        /// <summary>
        /// 胡牌检测(必须移除对子)
        /// </summary>
        /// <param name="handCards">检测手牌</param>
        /// <returns></returns>
        public static bool ChecHandCardHu(List<int> handCards)
        {
            if (handCards.Count == 0)//牌组数量为0 表示牌组全部符合标准
            {
                return true;
            }
            for (int j = 0; j < 4; j++)
            {
                //检测第一张牌，牌型为 AAA或者ABC 算通过，如果不是，则不能胡牌
                int cheeckCardValue = handCards[0];

                //获取当前牌的数量
                int checkLength = 4;
                if (checkLength > handCards.Count)
                {
                    checkLength = handCards.Count;
                }
                int cardCount = 1;
                for (int i = 1; i < checkLength; i++)
                {
                    if (cheeckCardValue == handCards[i])
                    {
                        cardCount++;
                    }
                    else
                    {
                        break;
                    }
                }

                if (cheeckCardValue > 30 && cheeckCardValue < 35)//唐山麻将风向牌必须是三暗刻才能移除  中华白算顺子
                {
                    if (cardCount == 3)
                    {
                        handCards.Remove(cheeckCardValue);
                        handCards.Remove(cheeckCardValue);
                        handCards.Remove(cheeckCardValue);
                    }
                }
                else
                {

                    switch (cardCount)
                    {
                        case 1://一张牌
                        case 2://两张牌
                               //移除ABC牌型
                            if (cheeckCardValue + 1 == handCards[cardCount])//判断B牌
                            {
                                //判断C牌 此处cardCount座位判断下标checkIndex
                                while (falg)
                                {
                                    cardCount++;
                                    if (cardCount >= handCards.Count)//超出范围 没找到C牌
                                    {
                                        return false;
                                    }

                                    if (handCards[cardCount - 1] != handCards[cardCount])//找到不相等的牌
                                    {
                                        break;
                                    }
                                }
                                if (cheeckCardValue + 2 == handCards[cardCount])//判断C牌
                                {
                                    handCards.Remove(cheeckCardValue);
                                    handCards.Remove(cheeckCardValue + 1);
                                    handCards.Remove(cheeckCardValue + 2);
                                }
                                else
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                return false;
                            }
                            break;
                        case 3://三张牌
                        case 4://四张牌
                               //先移除AAA牌型
                            handCards.Remove(cheeckCardValue);
                            handCards.Remove(cheeckCardValue);
                            handCards.Remove(cheeckCardValue);
                            break;

                    }
                }
                if (handCards == null || handCards.Count == 0)//牌组数量为0 表示牌组全部符合标准
                {
                    return true;
                }
            }
            return false;
            // return checkHuWithHandCards(handCards);
        }
    }
}
