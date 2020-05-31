#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <experimental/filesystem>
#include <map>
#include <algorithm>
#include <QTableWidget>
#include <QProgressBar>
#include<QDebug>


namespace fs = std::experimental::filesystem;
using namespace std;
ifstream file;

string path ;/* = "C:/Users/Aly EL-kady/Desktop/questions";*/


class BST {

    struct node {
        string word;
        vector<string> filename;
        node* left;
        node* right;
    };

    node* root;
    node* root2 ;

    node* insert(string myword , string myfilename, node* t)
    {
        if (t == NULL)
        {
            t = new node;
            t->word = myword;
            t->filename.push_back(myfilename);
            t->left = t->right = NULL;
        }
        else if (myword < t->word)
            t->left = insert(myword, myfilename, t->left);
        else if (myword > t->word)
            t->right = insert(myword, myfilename, t->right);
        else if (myword == t->word)
            t->filename.push_back(myfilename);
        return t;
    }
    node* find(node* t, string re_word) {

        if (t == NULL)
            return NULL;
        else if (re_word < t->word)
            return find(t->left, re_word);
        else if (re_word > t->word)
            return find(t->right, re_word);
        else
            return t;
    }
    void inorder(node* t) {
        if (t == NULL)
            return;
        inorder(t->left);
        cout << t->word << " " << endl;
        for (int i = 0; i < t->filename.size(); i++)
            cout << t->filename[i] << " " << endl;
        inorder(t->right);
    }


public:
    BST() {
        root = NULL;
    }

    void insert(string x , string filename) {
        root = insert(x, filename, root);
    }

    void display() {
        inorder(root);
        cout << endl;
    }

    vector <string> search(string x) {

        root2 = find(root, x);
        //root = find(root, x);
                vector<string> r = { "NOT-FOUND" };               
                if(root2!= NULL)
                return root2->filename;
                else return r;
    }
};



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setVisible(false);
    ui->progressBar->setVisible(false);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);

    ui->lineEdit_path->setVisible(true);
    ui->label_2->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}



 string keyWord;
 BST tree;
 float counter = 1;


 //C:\Users\Aly EL-kady\Desktop\aly - > 8files
 //C:\Users\Aly EL-kady\Desktop\questions - > 100k
 //C:\Users\Aly EL-kady\Desktop\dir - > 10k


void MainWindow::on_pushButton_clicked()
{
	vector<string> Final;

        for (int i = 0 ; i < Final.size(); i++)
        Final.erase(Final.begin());

       vector <string> words;
       vector <string> filenames;

        keyWord = ui->lineEdit->text().toStdString();

       for (int i = 0; i < keyWord.size(); i++)
       {
           keyWord[i]=tolower(keyWord[i]);
       }

       Final = tree.search(keyWord); //returns the vector

        for (int i = 0 ; i < Final.size(); i++){

           int row = ui->tableWidget->rowCount();
           ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row , 0 , new QTableWidgetItem(QString::fromStdString(Final[i])));
//            qApp->processEvents();
       }


       ui->tableWidget->sortItems(0 , Qt::SortOrder::AscendingOrder);
}

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    ui->textBrowser->setVisible(true);

    auto textt = ui->tableWidget->item(ui->tableWidget->currentRow(),0);
    QString text = textt->text();
    int comp;

    for (const auto& entry : fs::directory_iterator(path))
    {
        string temp_path = entry.path().string();
        char c = '\\';
        int index = temp_path.find_last_of(c);
        string filename = temp_path.substr(index + 1);
        comp = text.toStdString().compare(filename);
        if(comp==0)
        {
            QFile files(QString::fromStdString(temp_path));
                if(!files.open(QIODevice::ReadOnly))
                {
                    QMessageBox::information(0,"Info",files.errorString());
                }

                    QTextStream in (&files);
                    ui->textBrowser->setText(in.readAll());
                    break;
        }
    }

}

void MainWindow::on_lineEdit_editingFinished()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
    ui->textBrowser->hide();
    ui->progressBar->setVisible(true);
}

void MainWindow::on_lineEdit_path_editingFinished()
{
    ui->progressBar->setVisible(true);

     string word;
    path = ui->lineEdit_path->text().toStdString();

    for (int i = 0 ; i < path.length(); i++){
        if (path[i] == '\\' )
        {
           char c = '//';
           path[i]=c;
        }
    }

        float numFiles=0;
         for (const auto& entry : fs::directory_iterator(path)) {
             numFiles++;
         }

     for (const auto& entry : fs::directory_iterator(path)) {


         file.open(entry.path().string());

         string temp_path = entry.path().string();
         char c = '\\';
         int index = temp_path.find_last_of(c);
         string filename = temp_path.substr(index + 1);

         while (file >> word) {

             for (int i = 0; i < word.size(); i++)
             {
                 word[i]=tolower(word[i]);

             }
             /////////////tree-building/////////////////

             tree.insert(word , filename);

         }

                      float d,h;
                      d = 100/numFiles;
                      h = counter*d;
                     ui->progressBar->setValue(h);

                     counter++;

                    qApp->processEvents();

         file.close();
    }

     int x = 0;
     //QMessageBox :: information (this , "this" , QString::number(numFiles));
}
