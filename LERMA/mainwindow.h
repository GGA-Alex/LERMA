#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <vector>
#include <User.h>
#include <regex>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QGridLayout>
#include <QStringList>
#include <algorithm>
#include <Product.h>
#include <graph.h>
#include <queue>
#include <Nodo.h>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_usernameLE_textChanged(const QString &arg1);

    void on_passwordLE_textChanged(const QString &arg1);

    void on_newUserLE_textChanged(const QString &arg1);

    void on_newEmailLE_textChanged(const QString &arg1);

    void on_newPasswordLE_textChanged(const QString &arg1);

    void on_signUpPB_clicked();

    void on_loginPB_clicked();

    void openFile();

    void on_Products_currentTextChanged(const QString &arg1);

    void on_SortProducts_currentTextChanged(const QString &arg1);

    void on_Search_textChanged(const QString &arg1);

    void addToChart(QString item, int amount);

private:
    Ui::MainWindow *ui;

    vector<User> users;
    vector<QJsonArray> id_grafo;
    QString usuarioLogeado;

    vector<Product*> allProducts;
    vector<Product*> AB;
    vector<Product*> L;
    vector<Product*> E;
    vector<Product*> HC;
    vector<Product*> D;
    vector<Product*> actual;
    vector<Product*> busqueda;

    Graph<string> graph;

    QAction *openFileAction;
    QFile dbFile;

    QJsonArray dbArrayUser, dbArrayProduct;

    vector<QString> dbArrayPurchaseId;
    vector<int> dbArrayPurchaseCantidad;
    bool compro = false;

    time_t t;
    struct tm *tm;
    char fecha[100];
    char hora[100];
    QString fechaActual;

    QGridLayout *lay = new QGridLayout();

    QGridLayout *recomendados = new QGridLayout();
    priority_queue<Nodo*, vector<Nodo*>, Nodo::comparador> productosAcomodados;

    bool sort = false;
    void enabledLoginPB();
    void enabledSignUpPb();
    void validateUser();
    bool validateEmail(const QString &email);
    bool verifyUser(const QString &new_user);
    bool verifyEmail(const QString &new_email);
    void Productos(QString current_text, bool actualizar);
    void ProductosAleatoriosInicial();
    bool existeNumero(int numero, int numeros[]);
    void ProductosRecomendados();
    void SortGeneral(QString currentTextSort);
    void SortSearch(QString currentTextSort);
    void eliminar();
    void createGraph();

    void saveDB();
    void loadDB();
};

#endif // MAINWINDOW_H
