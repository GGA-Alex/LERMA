#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    openFileAction = new QAction("&Open Database", this);
    connect(openFileAction, SIGNAL(triggered()),this, SLOT(openFile()));
    ui->menuBar->addMenu("&File")->addAction(openFileAction);
    ui->scrollArea->setWidgetResizable(true);
    ui->Recomendaciones->setWidgetResizable(true);
}

MainWindow::~MainWindow()
{
    saveDB();  //Función que se encarga de guardar los datos de los usuarios y los productos
    delete ui;
}

void MainWindow::enabledLoginPB(){
    if (ui->usernameLE->text().length() > 0 && ui->passwordLE->text().length() > 0)
        ui->loginPB->setEnabled(true);
    else
        ui->loginPB->setEnabled(false);
}

void MainWindow::enabledSignUpPb()
{
    if (ui->newUserLE->text().length() > 0 && ui->newPasswordLE->text().length() > 0 && ui->newEmailLE->text().length() > 0)
        ui->signUpPB->setEnabled(true);
    else
        ui->signUpPB->setEnabled(false);
}

void MainWindow::validateUser()
{
    QMessageBox message;
    vector<User>::iterator it;
    QString user = ui->usernameLE->text();
    QString password = ui->passwordLE->text();
    message.setStyleSheet("QLabel{font: 10.5pt ""Yu Gothic UI"";}"
                          "QPushButton{ font: 75 12pt ""MS Shell Dlg 2""; "
                          "color: rgb(255, 255, 255); background-color: rgb(185, 15, 43); "
                          "border-radius: 10px; width: 80px; height: 30px;}");

    it = find_if(users.begin(),users.end(), [&user, &password](User u) -> bool
    {
        return u.getUsername() == user && u.getPassword() == password;
    }
    );

    if (it == users.end()){
        message.setText("Invalid credentials");
        message.setIcon(QMessageBox::Warning);
        message.exec();
    }
    else{
        message.setText("Welcome to LERMA " + user);
        usuarioLogeado = user;
        ui->viewSW->setCurrentIndex(1);
        Productos("Todos los Departamentos",false);
        ProductosAleatoriosInicial();
        message.setStyleSheet("QLabel{font: 10.5pt ""Yu Gothic UI"";}"
                              "QPushButton{ font: 75 12pt ""MS Shell Dlg 2""; "
                              "color: rgb(255, 255, 255); background-color:  rgb(0, 120, 187); "
                              "border-radius: 10px; width: 80px; height: 30px;}");
        message.exec();
    }
}

bool MainWindow::validateEmail(const QString &email)
{
    regex match("^([A-Za-z\\d\\.\\-_]+)@([a-zA-Z]+)\\.([a-zA-Z]{3})$");

    return regex_match(email.toStdString(),match);
}

bool MainWindow::verifyUser(const QString &new_user)
{
    vector<User>::iterator it;

    it = find_if(users.begin(),users.end(), [&new_user](User u) -> bool
    {
        return u.getUsername() == new_user;
    }
    );

    if (it == users.end()){
        return false;
    }
    else{
       return true;
    }
}

bool MainWindow::verifyEmail(const QString &new_email)
{
    vector<User>::iterator it;

    it = find_if(users.begin(),users.end(), [&new_email](User u) -> bool
    {
        return u.getEmail() == new_email;
    }
    );
    if (it == users.end()){
        return false;
    }
    else{
       return true;
    }
}

void MainWindow::on_usernameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enabledLoginPB();
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enabledLoginPB();
}

void MainWindow::on_newUserLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enabledSignUpPb();
}

void MainWindow::on_newEmailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enabledSignUpPb();
}

void MainWindow::on_newPasswordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enabledSignUpPb();
}

void MainWindow::on_signUpPB_clicked()
{
    QJsonObject jsonObj;
    QMessageBox message;
    User u;
    message.setStyleSheet("QLabel{font: 10.5pt ""Yu Gothic UI"";}"
                          "QPushButton{ font: 75 12pt ""MS Shell Dlg 2""; "
                          "color: rgb(255, 255, 255); background-color: rgb(185, 15, 43); "
                          "border-radius: 10px; width: 80px; height: 30px;}");    

    if(validateEmail(ui->newEmailLE->text())){
        if(!verifyUser(ui->newUserLE->text())){
            if(!verifyEmail(ui->newEmailLE->text())){
                u.setUsername(ui->newUserLE->text());
                u.setEmail(ui->newEmailLE->text());
                u.setPassword(ui->newPasswordLE->text());

                users.push_back(u);

                message.setText("New user created.");
                message.setStyleSheet("QLabel{font: 10.5pt ""Yu Gothic UI"";}"
                                      "QPushButton{ font: 75 12pt ""MS Shell Dlg 2""; "
                                      "color: rgb(255, 255, 255); background-color:  rgb(0, 120, 187); "
                                      "border-radius: 10px; width: 80px; height: 30px;}");
                message.exec();

                jsonObj["name"] = u.getUsername();
                jsonObj["email"] = u.getEmail();
                jsonObj["password"] = u.getPassword();
                dbArrayUser.append(jsonObj);
            }
            else{
                message.setText("This Email already exists.");
                message.exec();
            }
        }
        else{
            message.setText("This User already exists.");
            message.exec();
        }

    }else{
        message.setText("Invalid Email.");
        message.exec();
    }


    ui->newUserLE->clear();
    ui->newEmailLE->clear();
    ui->newPasswordLE->clear();


}

void MainWindow::on_loginPB_clicked()
{
    validateUser();

    ui->usernameLE->clear();
    ui->passwordLE->clear();
}

void MainWindow::openFile()
{
    QString name;
    name = QFileDialog::getOpenFileName(this,
                                        "Select DataBase",
                                        "",
                                        "JSON files (*.json)");
    if (name.length() > 0){
        dbFile.setFileName(name);

        ui->loginGB->setEnabled(true);
        ui->signUpGB->setEnabled(true);
        loadDB();
    }
}

void MainWindow::saveDB()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QString dateP, timeP;

    t=time(NULL);
    tm=localtime(&t);
    strftime(hora, 100, "%H:%M:%S", tm);
    strftime(fecha, 100, "%d/%m/%Y", tm);

    dateP = fecha;
    timeP = hora;
    fechaActual = dateP + " " + timeP;

    if(compro){
        for(int i = 0; i < users.size(); i++){
            if(users.at(i).getUsername() == usuarioLogeado){
                QJsonObject usuario = dbArrayUser.at(i).toObject();
                QJsonArray compraAnterior = usuario["purchase"].toArray();

                QJsonArray nuevaCompra;
                QJsonObject fechaCompra;
                QJsonObject Guardar;

                for(size_t j = 0; j < dbArrayPurchaseId.size(); j++){
                    Guardar.insert("id",QJsonValue::fromVariant(dbArrayPurchaseId.at(j)));
                    Guardar.insert("Cantidad",QJsonValue::fromVariant(dbArrayPurchaseCantidad.at(j)));
                    nuevaCompra.push_back(Guardar);
                }

                fechaCompra.insert(fechaActual,nuevaCompra);
                compraAnterior.push_back(fechaCompra);

                usuario.insert("purchase",compraAnterior);

                dbArrayUser.removeAt(i);
                dbArrayUser.insert(i,usuario);

                }
            }
    }

    jsonObj["users"] = dbArrayUser;
    jsonObj["products"] = dbArrayProduct;

    jsonDoc= QJsonDocument(jsonObj);

    dbFile.open(QIODevice::WriteOnly);
    dbFile.write(jsonDoc.toJson());
    dbFile.close();
}

void MainWindow::loadDB()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray data;
    regex AB("^([AB\\d]+)$"), L("^([L\\d]+)$"), E("^([E\\d]+)$"), HC("^([HC\\d]+)$"), D("^([D\\d]+)$");

    dbFile.open(QIODevice::ReadOnly);
    data = dbFile.readAll();
    dbFile.close();

    jsonDoc = QJsonDocument::fromJson(data);
    jsonObj = jsonDoc.object();

    dbArrayUser = jsonObj["users"].toArray();
    dbArrayProduct = jsonObj["products"].toArray();

    for (int i(0); i < dbArrayUser.size(); ++i){
        User u;
        QJsonObject obj = dbArrayUser[i].toObject();

        u.setUsername(obj["name"].toString());
        u.setEmail(obj["email"].toString());
        u.setPassword(obj["password"].toString());

        users.push_back(u);
    }

    for (int i(0); i < dbArrayProduct.size(); ++i){
        Product *pw = new Product();
        QJsonObject obj = dbArrayProduct[i].toObject();

        pw->setId(obj["id"].toString());
        pw->setName(obj["name"].toString());
        pw->setPriceProduct(obj["price"].toDouble());

        if(regex_match(pw->getId().toStdString(),AB))
            this->AB.push_back(pw);
        else if (regex_match(pw->getId().toStdString(),L))
            this->L.push_back(pw);
        else if (regex_match(pw->getId().toStdString(),E))
            this->E.push_back(pw);
        else if (regex_match(pw->getId().toStdString(),HC))
            this->HC.push_back(pw);
        else
            this->D.push_back(pw);

        allProducts.push_back(pw);
    }
    createGraph();
}

void MainWindow::on_Products_currentTextChanged(const QString &arg1)
{
    Productos(arg1,false);
}

void MainWindow::Productos(QString current_text, bool actualizar)
{
    int col = 0, row = 0;
    string valor;
    if(!actualizar){
        ui->SortProducts->setCurrentIndex(0);
        ui->Search->setText("");
        actual.clear();
    }
    eliminar();
    if(current_text == "Todos los Departamentos" && actualizar == false){
        for (size_t i = 0; i < allProducts.size(); i++){
            if(col == 4){
                col = 0;
                row++;
            }
            Product *pw = new Product();
            pw->setId(allProducts[i]->getId());
            pw->setName(allProducts[i]->getName());
            pw->setPriceProduct(allProducts[i]->getPriceProduct());
            actual.push_back(allProducts[i]);
            connect(pw,SIGNAL(addItem(QString,int)), this, SLOT(addToChart(QString,int)));
            lay->addWidget(pw,row,col);
            col++;
        }
    }
    else if(current_text == "Alimentos y Bebidas" && actualizar == false){
        for (size_t i = 0; i < AB.size(); i++){
            if(col == 4){
                col = 0;
                row++;
            }
            Product *pw = new Product();
            pw->setId(AB[i]->getId());
            pw->setName(AB[i]->getName());
            pw->setPriceProduct(AB[i]->getPriceProduct());
            actual.push_back(AB[i]);
            connect(pw,SIGNAL(addItem(QString,int)), this, SLOT(addToChart(QString,int)));
            lay->addWidget(pw,row,col);
            col++;
        }
    }
    else if(current_text == "Libros" && actualizar == false){
        for (size_t i = 0; i < L.size(); i++){
            if(col == 4){
                col = 0;
                row++;
            }
            Product *pw = new Product();
            pw->setId(L[i]->getId());
            pw->setName(L[i]->getName());
            pw->setPriceProduct(L[i]->getPriceProduct());
            actual.push_back(L[i]);
            connect(pw,SIGNAL(addItem(QString,int)), this, SLOT(addToChart(QString,int)));
            lay->addWidget(pw,row,col);
            col++;
        }
    }
    else if(current_text == "Electronicos" && actualizar == false){
        for (size_t i = 0; i < E.size(); i++){
            if(col == 4){
                col = 0;
                row++;
            }
            Product *pw = new Product();
            pw->setId(E[i]->getId());
            pw->setName(E[i]->getName());
            pw->setPriceProduct(E[i]->getPriceProduct());
            actual.push_back(E[i]);
            connect(pw,SIGNAL(addItem(QString,int)), this, SLOT(addToChart(QString,int)));
            lay->addWidget(pw,row,col);
            col++;
        }
    }
    else if(current_text == "Hogar y Cocina" && actualizar == false){
        for (size_t i = 0; i < HC.size(); i++){
            if(col == 4){
                col = 0;
                row++;
            }
            Product *pw = new Product();
            pw->setId(HC[i]->getId());
            pw->setName(HC[i]->getName());
            pw->setPriceProduct(HC[i]->getPriceProduct());
            actual.push_back(HC[i]);
            connect(pw,SIGNAL(addItem(QString,int)), this, SLOT(addToChart(QString,int)));
            lay->addWidget(pw,row,col);
            col++;
        }
    }
    else if(current_text == "Deporte y Aire Libre" && actualizar == false){
        for (size_t i = 0; i < D.size(); i++){
            if(col == 4){
                col = 0;
                row++;
            }
            Product *pw = new Product();
            pw->setId(D[i]->getId());
            pw->setName(D[i]->getName());
            pw->setPriceProduct(D[i]->getPriceProduct());
            actual.push_back(D[i]);
            connect(pw,SIGNAL(addItem(QString,int)), this, SLOT(addToChart(QString,int)));
            lay->addWidget(pw,row,col);
            col++;
        }
    }
    else if(current_text == "Actualizar" && actualizar == true){
        for (size_t i = 0; i < actual.size(); i++){
            if(col == 4){
                col = 0;
                row++;
            }
            Product *pw = new Product();
            pw->setId(actual[i]->getId());
            pw->setName(actual[i]->getName());
            pw->setPriceProduct(actual[i]->getPriceProduct());
            connect(pw,SIGNAL(addItem(QString,int)), this, SLOT(addToChart(QString,int)));
            lay->addWidget(pw,row,col);
            col++;
        }
    }
    else if(current_text == "Buscar" && actualizar == true){
           for (size_t i = 0; i < busqueda.size(); i++){
               if(col == 4){
                   col = 0;
                   row++;
               }
               Product *pw = new Product();
               pw->setId(busqueda[i]->getId());
               pw->setName(busqueda[i]->getName());
               pw->setPriceProduct(busqueda[i]->getPriceProduct());
               connect(pw,SIGNAL(addItem(QString,int)), this, SLOT(addToChart(QString,int)));
               lay->addWidget(pw,row,col);
               col++;
           }
       }

    ui->scrollProducts->setLayout(lay);
}

void MainWindow::ProductosAleatoriosInicial()
{
    QLayoutItem *child;
    while ((child = recomendados->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    srand(time(NULL));
    int numeros[5];
    int posicion;
    for(int i = 0; i < 5; i++){
        do{
            posicion = rand() % allProducts.size();
        }while(existeNumero(posicion,numeros));
        numeros[i] = posicion;
        Product *pw = new Product();
        pw->setId(allProducts[posicion]->getId());
        pw->setName(allProducts[posicion]->getName());
        pw->setPriceProduct(allProducts[posicion]->getPriceProduct());
        recomendados->addWidget(pw,0,i);
    }
    ui->scrollRecomendaciones->setLayout(recomendados);

}

bool MainWindow::existeNumero(int numero, int numeros[])
{
    for(int i = 0; i < 5; i++){
        if(numeros[i] == numero)
            return true;
    }
    return false;
}

void MainWindow::ProductosRecomendados()
{
    QLayoutItem *child;
    while ((child = recomendados->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    int columna = 0;
    qDebug() << "-----------------------------------------" << endl;
    priority_queue<Nodo*, vector<Nodo*>, Nodo::comparador> copia;
    while(!productosAcomodados.empty()){
        copia.push(productosAcomodados.top());
        productosAcomodados.pop();
    }
    while (!copia.empty()) {
        for(size_t i = 0; i < allProducts.size(); i++){
            Nodo *nodo = copia.top();
            Product *pw = new Product();
            if(allProducts[i]->getId() == QString::fromStdString(nodo->getId()) && (columna < 5)){
                pw->setId(allProducts[i]->getId());
                pw->setName(allProducts[i]->getName());
                pw->setPriceProduct(allProducts[i]->getPriceProduct());
                recomendados->addWidget(pw,0,columna);
                columna++;
                break;
            }
        }
        productosAcomodados.push(copia.top());
        cout << "ID: " << copia.top()->getId() << " - Peso:  " << copia.top()->getPeso() << endl;
        copia.pop();
    }
    ui->scrollRecomendaciones->setLayout(recomendados);
}

void MainWindow::on_SortProducts_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Ascendente" || arg1 == "Descendente")
        SortGeneral(arg1);
    if((arg1 == "Ascendente" || arg1 == "Descendente") && ui->Search->text().length() > 0)
        SortSearch(arg1);
}

void MainWindow::SortGeneral(QString currentTextSort)
{
    if(currentTextSort == "Ascendente"){
        for(size_t i=0; i<actual.size() ; i++){
            for(size_t j=i+1; j< actual.size(); j++){
                if(actual[j]->getPriceProduct() < actual[i]->getPriceProduct()){
                    Product *aux=actual[i];
                    actual[i]=actual[j];
                    actual[j]=aux;
                }
            }
        }
    }
    if(currentTextSort == "Descendente"){
        for(size_t i=0; i<actual.size() ; i++){
            for(size_t j=i+1; j< actual.size(); j++){
                if(actual[j]->getPriceProduct() > actual[i]->getPriceProduct()){
                    Product *aux=actual[i];
                    actual[i]=actual[j];
                    actual[j]=aux;
                }
            }
        }
    }
    Productos("Actualizar",true);
}

void MainWindow::SortSearch(QString currentTextSort)
{
    if(currentTextSort == "Ascendente"){
        for(size_t i=0; i<busqueda.size() ; i++){
            for(size_t j=i+1; j< busqueda.size(); j++){
                if(busqueda[j]->getPriceProduct() < busqueda[i]->getPriceProduct()){
                    Product *aux=busqueda[i];
                    busqueda[i]=busqueda[j];
                    busqueda[j]=aux;
                }
            }
        }
    }
    if(currentTextSort == "Descendente"){
        for(size_t i=0; i<busqueda.size() ; i++){
            for(size_t j=i+1; j< busqueda.size(); j++){
                if(busqueda[j]->getPriceProduct() > busqueda[i]->getPriceProduct()){
                    Product *aux=busqueda[i];
                    busqueda[i]=busqueda[j];
                    busqueda[j]=aux;
                }
            }
        }
    }
    Productos("Buscar",true);
}

void MainWindow::eliminar()
{
    QLayoutItem *child;
    while ((child = lay->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
}

void MainWindow::createGraph()
{
    for(int i = 0; i < dbArrayUser.size(); ++i){
        QJsonArray obj = dbArrayUser.at(i).toObject()["purchase"].toArray();
        for(int i = 0; i < obj.size(); i++){
            for(auto fecha_compra : obj.at(i).toObject().keys()){
                vector<QString> aristas;
                QJsonValue productos = obj.at(i).toObject()[fecha_compra];
                if(productos.toArray().size() > 1){
                    for(int i = 0; i < productos.toArray().size(); i++)
                        aristas.push_back(productos[i].toObject()["id"].toString());

                    for(size_t i = 0; i < aristas.size(); i++){
                        for(size_t j = i + 1; j < aristas.size(); j++){
                            if(graph.isEdge(aristas[i].toStdString(),aristas[j].toStdString()))
                                graph.createEdge(aristas[i].toStdString(),aristas[j].toStdString(),graph.getCost(aristas[i].toStdString(),aristas[j].toStdString()) + 1);
                            else
                                graph.createEdge(aristas[i].toStdString(),aristas[j].toStdString(),1);
                        }
                    }
                }
            }
        }
    }
    graph.printData();
}

void MainWindow::on_Search_textChanged(const QString &arg1)
{
    int col = 0, row = 0;
    regex match(arg1.toStdString(),regex_constants::icase);
    eliminar();
    busqueda.clear();
    for (size_t i = 0; i < actual.size(); i++){
        if(col == 4){
            col = 0;
            row++;
        }
        if(regex_search(actual[i]->getName().toStdString(),match)){
            Product *pw = new Product();
            pw->setId(actual[i]->getId());
            pw->setName(actual[i]->getName());
            pw->setPriceProduct(actual[i]->getPriceProduct());
            connect(pw,SIGNAL(addItem(QString,int)), this, SLOT(addToChart(QString,int)));
            lay->addWidget(pw,row,col);
            col++;
            busqueda.push_back(actual[i]);
        }
    }
    ui->scrollProducts->setLayout(lay);
}

void MainWindow::addToChart(QString item, int amount)
{
    string id_obtenido;
    compro = true;
    dbArrayPurchaseId.push_back(item);
    dbArrayPurchaseCantidad.push_back(amount);

    if(graph.contains(item.toStdString())){
        typename map<string, int>::const_iterator it;
        it = graph.getNeighbors(item.toStdString()).begin();
        while(it != graph.getNeighbors(item.toStdString()).end()){
            Nodo* nodo = new Nodo();
            nodo->setId(it->first);
            nodo->setPeso(it->second);
            productosAcomodados.push(nodo);
            it++;
        }
        ProductosRecomendados();
    }

}
