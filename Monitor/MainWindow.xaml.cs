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

namespace Monitor
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    public MainWindow()
    {
      InitializeComponent();
    }

    public int count = 0;

    private void ServerTextBox_TextChanged(object sender, TextChangedEventArgs e)
    {

    }

    private void Button_Click_2(object sender, RoutedEventArgs e)
    {
      TextBox a = new TextBox();
      a.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;
      a.Width = 175;
      a.Text = "Testing";
      a.VerticalAlignment = VerticalAlignment.Stretch;
      this.MainStackPanel.Children.Add(a);
    }
  }
}
