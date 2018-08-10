using System;
using System.Collections.Specialized;
using System.Diagnostics;
using System.Net;
using System.Text;

namespace xx
{
    /// <summary>
    /// 实现一个简易的嵌入式 web server.  示例在最下面
    /// </summary>
    public class HttpServer
    {
        HttpListener httpListener = new HttpListener();

        // windows 下一般用户需要执行这句来注册添加权限
        // netsh http add urlacl url=http://*:80/ user=Everyone listen=yes
        void SetAuth(string urlPrefix)
        {
            var psi = new ProcessStartInfo("netsh", $"http add urlacl {urlPrefix} user=Everyone listen=yes")
            {
                Verb = "runas",
                CreateNoWindow = true,
                WindowStyle = ProcessWindowStyle.Hidden,
                UseShellExecute = true
            };
            Process.Start(psi).WaitForExit();
        }

        void BeginGetContext()
        {
            httpListener.BeginGetContext(ar =>
            {
                var ctx = httpListener.EndGetContext(ar);
                BeginGetContext();
                Handle(ctx);
            }, null);
        }

        void Handle(HttpListenerContext ctx)
        {
            string outMsg = null;
            if (handlers.TryGetValue(ctx.Request.Url.AbsolutePath, out var handler))
            {
                try
                {
                    outMsg = handler(new UrlArgs(ctx.Request.QueryString));
                }
                catch (Exception ex)
                {
                    outMsg = "exception: " + ex.Message;
                }
            }

            var re = ctx.Response;
            re.StatusCode = (int)HttpStatusCode.OK;
            var buf = Encoding.UTF8.GetBytes(outMsg == null ? ("unhandled path:" + ctx.Request.Url.AbsolutePath) : outMsg);
            re.ContentEncoding = Encoding.UTF8;
            re.ContentType = "text/plain;charset=UTF-8";  //"text/xml";
            re.ContentLength64 = buf.Length;
            re.OutputStream.Write(buf, 0, buf.Length);
            re.Close();
        }

        /// <summary>
        /// 返回 null 表示成功. 
        /// </summary>
        public Exception Listen(int port = 80)
        {
            var urlPrefix = $"http://*:{port}/";
            SetAuth(urlPrefix);

            httpListener.AuthenticationSchemes = AuthenticationSchemes.Anonymous;
            httpListener.Prefixes.Add(urlPrefix);
            try
            {
                httpListener.Start();   // 有可能没权限运行
            }
            catch (Exception ex)
            {
                return ex;
            }
            BeginGetContext();
            return null;
        }

        // 以 AbsolutePath 为 key, 提供注册. 
        Dict<string, Func<UrlArgs, string>> handlers = new Dict<string, Func<UrlArgs, string>>();
        public void SetHandler(string absolutePath, Func<UrlArgs, string> handler)
        {
            if (handler == null)
            {
                handlers.Remove(absolutePath);
            }
            else
            {
                handlers.Add(absolutePath, handler);
            }
        }
    }

    public class UrlArgs
    {
        public xx.List<String> values = new xx.List<string>();
        public xx.Dict<String, String> keyValuePairs = new xx.Dict<string, string>();
        public UrlArgs(NameValueCollection nvc)
        {
            foreach (string k in nvc.AllKeys)
            {
                if (k == null)
                {
                    values.AddRange(nvc[null].Split(','));
                }
                else
                {
                    keyValuePairs.Add(k, nvc[k]);
                }
            }
        }

        // http://127.0.0.1/asdf.asdf?asdf=asdf&qwer&zxcv=123&ererer
        // 以上串为例, & 分割开的字串中, 有 asdf = asdf, zxcv = 123  是键值对, qwer, ererer 是 values 数组. 数组以逗号分割存在了 key 为 null 的 value 里
        public void DumpTo(StringBuilder outSb)
        {
            outSb.Append(values);
            keyValuePairs.ForEach(kv =>
            {
                outSb.AppendFormat("key:{0}, value:{1}\r\n", kv.key, kv.value);
            });
        }
    }

}

/*
public static void Main(string[] args)
{
    var hs = new xx.HttpServer();
    {
        var urlPath = "/xxx";
        hs.SetHandler(urlPath, urlArgs =>
        {
            // 参数识别
            string arg1 = null;
            if (!urlArgs.keyValuePairs.TryGetValue("arg1", out arg1))
            {
                return "arg1 is not found.";
            }

            // 处理
            // ...

            // 返回结果
            return "ok... got arg1 = " + arg1;
        });
    }
    // ... more

    int port = 8080;
    var ex = hs.Listen(port);
    if (ex == null)
    {
        Console.WriteLine($"http listing at {port}...");
    }
    else
    {
        Console.WriteLine(ex.Message);
    }
    Console.Read();
}
*/
