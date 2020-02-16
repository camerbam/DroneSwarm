using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.ComponentModel;
using global::Proto;

namespace Monitor
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    private bool running;
    private List<Thread> Threads;
    private Proto.StringMsg msg;

    private void ListenForMessages(TextBox boxToType, TcpClient client)
    {
      try
      {
        while (running)
        {
          NetworkStream stream = client.GetStream();
          Byte[] bytes = new Byte[1000];
          int i;

          // Loop to receive all the data sent by the client.
          while ((i = stream.Read(bytes, 0, 1)) != 0)
          {
            if (bytes[0] != 0x01) continue;
            if ((i = stream.Read(bytes, 0, 5)) == 0) continue;
            Int32 size = System.Convert.ToInt32(System.Text.Encoding.ASCII.GetString(bytes, 0, 5));
            if ((i = stream.Read(bytes, 0, size)) == 0) continue;

            msg = StringMsg.Parser.ParseFrom(bytes, 0, size);
            this.Dispatcher.Invoke(() =>
            {
              boxToType.Text += msg.Msg + "\n";
            });
          }
        }
      }
      catch(Exception)
      {
        this.Dispatcher.Invoke(() =>
        {
          this.MainStackPanel.Children.Remove(boxToType);
        });
      }
    }

    private Thread t;
    private TcpListener Server;
    private void ListenForConnections()
    {
      while(running)
      {
        TcpClient client = Server.AcceptTcpClient();

        this.Dispatcher.Invoke(() =>
        {
          TextBox a = new TextBox();
          a.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;
          a.TextWrapping = System.Windows.TextWrapping.Wrap;
          a.Width = 175;
          a.Text = "";
          a.VerticalAlignment = VerticalAlignment.Stretch;
          this.MainStackPanel.Children.Add(a);

          t = new Thread(() =>
          {
            ListenForMessages(a, client);
          });
          t.Start();
          Threads.Add(t);
        });
      }
    }

    public MainWindow()
    {
      InitializeComponent();
      running = true;
      Threads = new List<Thread>();
      Server = new TcpListener(IPAddress.Parse("127.0.0.1"), 13000);
      Server.Start();
      t = new Thread(new ThreadStart(ListenForConnections));
      t.SetApartmentState(ApartmentState.STA);
      t.Start();
    }

    public void DataWindow_Closing(object sender, CancelEventArgs e)
    {
      Console.WriteLine("This is C#");
      Server.Stop();
      t.Join();
    }

    ~MainWindow()
    {
    }
  }
}
