using System;

namespace xx
{
    // memory model: ring buffer
    // move to RIGHT way = forward
    // 3 situation:
    //...............FR...............		    // Head == Tail
    //......Head+++++++++++Tail.......		    // DataLen = Tail - Head
    //++++++Tail...........Head+++++++		    // DataLen = BufLen - Head + Tail
    public class Queue<T>
    {
        int head, tail;
        T[] array;

        /// <summary>
        /// 预申请 空间
        /// </summary>
        public Queue(int capacity)
        {
            if (capacity <= 0)
            {
                capacity = 8;
            }
            array = new T[capacity];
            // FR..............................
            head = 0;
            tail = 0;
        }

        /// <summary>
        /// 预申请 默认 空间
        /// </summary>
        public Queue() : this(8)
        {
        }

        /// <summary>
        /// 返回元素个数
        /// </summary>
        public int Count
        {
            get
            {
                //......Head+++++++++++Tail.......
                //...............FR...............
                if (head <= tail)
                {
                    return tail - head;
                }
                // ++++++Tail...........Head++++++
                else
                {
                    return tail + (array.Length - head);
                }
            }
        }

        /// <summary>
        /// 是否没有数据
        /// </summary>
        public bool IsEmpty
        {
            get
            {
                return head == tail;
            }
        }

        /// <summary>
        /// 取出一个元素并返回
        /// </summary>
        public T Dequeue()
        {
            if (head == tail)
                throw new InvalidOperationException();

            T removed = array[head];
            array[head++] = default(T);
            if (head == array.Length)
            {
                head = 0;
            }
            return removed;
        }

        /// <summary>
        /// 试取出一个元素, 成功则返回 true
        /// </summary>
        public bool TryDequeue(out T outVal)
        {
            if (head == tail)
            {
                outVal = default(T);
                return false;
            }
            outVal = array[head];
            array[head++] = default(T);
            if (head == array.Length)
            {
                head = 0;
            }
            return true;
        }

        /// <summary>
        /// 读取一个即将取出的元素
        /// </summary>
        public T Peek()
        {
            if (head == tail)
                throw new InvalidOperationException();

            return array[head];
        }

        /// <summary>
        /// 读取一个即将取出的元素
        /// </summary>
        public T Top()
        {
            if (head == tail)
                throw new InvalidOperationException();

            return array[head];
        }


        /// <summary>
        /// 放入一个元素
        /// </summary>
        public void Enqueue(T v)
        {
            array[tail++] = v;
            if (tail == array.Length)   // cycle
            {
                tail = 0;
            }
            if (tail == head)          // no more space
            {
                Resize();
            }
        }

        /// <summary>
        /// 放入一个元素
        /// </summary>
        public void Push(T v)
        {
            array[tail++] = v;
            if (tail == array.Length)   // cycle
            {
                tail = 0;
            }
            if (tail == head)          // no more space
            {
                Resize();
            }
        }


        /// <summary>
        /// 清空所有元素
        /// </summary>
        public void Clear()
        {
            if (head == tail)
            {
                return;
            }
            // ......Head+++++++++++Tail......
            else if (head < tail)
            {
                Array.Clear(array, head, Count);
            }
            // ++++++Tail...........Head++++++
            else
            {
                Array.Clear(array, head, array.Length - head);
                Array.Clear(array, 0, tail);
            }
            // ........HT......................
            head = tail = 0;
        }

        /// <summary>
        /// 取出一个元素抛弃掉( 通常与 Peek 配合使用 )
        /// </summary>
        public void Pop()
        {
            if (head == tail)
                throw new InvalidOperationException();

            array[head++] = default(T);
            if (head == array.Length)
            {
                head = 0;
            }
        }

        /// <summary>
        /// 取出 count 个元素抛弃掉
        /// </summary>
        public int PopMulti(int count)
        {
            if (count <= 0)
            {
                return 0;
            }
            var dataLen = Count;
            if (count >= dataLen)
            {
                Clear();
                return dataLen;
            }
            // count < dataLen

            // ......Head+++++++++++Tail......
            if (head < tail)
            {
                Array.Clear(array, head, count);
                head += count;
            }
            // ++++++Tail...........Head++++++
            else
            {
                var frontDataLen = array.Length - head;
                //...Head+++
                if (count < frontDataLen)
                {
                    Array.Clear(array, head, count);
                    head += count;
                }
                else
                {
                    //...Head++++++
                    Array.Clear(array, head, array.Length - head);
                    // ++++++Tail...
                    head = count - frontDataLen;
                    Array.Clear(array, 0, head);
                }
            }
            return count;
        }

        /// <summary>
        /// 随机检索 指定下标的元素
        /// </summary>
        public T this[int index]
        {
            get
            {
                if (head + index < array.Length)
                {
                    return array[head + index];
                }
                else
                {
                    return array[head + index - array.Length];
                }
            }

            set
            {
                array[(index + head) % array.Length] = value;
                if (head + index < array.Length)
                {
                    array[head + index] = value;
                }
                else
                {
                    array[head + index - array.Length] = value;
                }
            }
        }



        void Resize()
        {
            var newarray = new T[array.Length * 2];

            //......Head+++++++++++Tail.......
            if (head < tail)
            {
                Array.Copy(array, head, newarray, 0, array.Length);
            }
            // ++++++Tail...........Head+++++++
            // ++++++++++++++TH++++++++++++++++
            else
            {
                //...Head++++++
                Array.Copy(array, head, newarray, 0, array.Length - head);
                // ++++++Tail...
                Array.Copy(array, 0, newarray, array.Length - head, tail);
            }

            // Head+++++++++++Tail.............
            head = 0;
            tail = array.Length;

            array = newarray;
        }


        /// <summary>
        /// 逻辑上是将 pop 的这个元素立即 enqueue 进去 并返回.
        /// </summary>
        public T EnqueuePop()
        {
            if (head == tail) throw new InvalidOperationException();

            var rtv = array[tail] = array[head];
            array[head] = default(T);

            ++head;
            if (head == array.Length)
            {
                head = 0;
            }

            ++tail;
            if (tail == array.Length)   // cycle
            {
                tail = 0;
            }

            return rtv;
        }
    };
}
