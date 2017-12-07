using System;

public class XxSimpleList<T>
{
    public T[] buf;
    public int bufLen { get { return buf.Length; } }
    public int dataLen;

    private const int defaultCapacity = 4;
    static readonly T[] emptyBuf = new T[0];

    public XxSimpleList(int capacity = 0)
    {
        buf = emptyBuf;
        Reserve(capacity);
    }

    public T this[int index]
    {
        get { return buf[index]; }
        set { buf[index] = value; }
    }

    public void Add(T item)
    {
        if (dataLen == buf.Length) Reserve(dataLen + 1);
        buf[dataLen++] = item;
    }

    public void Reserve(int min)
    {
        if (buf.Length < min)
        {
            int newCapacity = buf.Length == 0 ? defaultCapacity : buf.Length * 2;
            if (newCapacity < min) newCapacity = min;
            if (newCapacity < buf.Length) return;
            T[] newItems = new T[newCapacity];
            if (dataLen > 0)
            {
                Array.Copy(buf, 0, newItems, 0, dataLen);
            }
            buf = newItems;
        }
    }

    public void SwapRemoveAt(int index)
    {
        if (index + 1 < dataLen)
        {
            buf[index] = buf[dataLen - 1];
        }
        dataLen--;
        buf[dataLen] = default(T);
    }

    public void ForEach(Action<T> action)
    {
        for (int i = dataLen - 1; i >= 0; --i)
        {
            action(buf[i]);
        }
    }
}
