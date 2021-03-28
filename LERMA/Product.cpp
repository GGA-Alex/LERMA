#include "Product.h"
#include "ui_Product.h"


Product::Product(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Product)
{
    ui->setupUi(this);
}

Product::~Product()
{
    delete ui;
}

QString Product::getId() const
{
    return idProduct;
}

void Product::setId( QString value)
{
    idProduct = value;
    QPixmap p = ":\\imgs\\" + idProduct + ".jpg";
    int w = 200;
    int h = 200;
    ui->img->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));

}

QString Product::getName() const
{
    return nameProduct;
}

void Product::setName(QString value)
{
    nameProduct = value;
    ui->name->setText(nameProduct);
}

void Product::setImage()
{
    ui->img->setText(idProduct);
}

float Product::getPriceProduct() const
{
    return priceProduct;
}

void Product::setPriceProduct(float value)
{
    priceProduct = value;
     ui->price->setText("$" + QString::number(priceProduct));
}

void Product::on_sp_Cantidad_valueChanged(int arg1)
{
    (arg1 > 0) ? ui->btn_Agregar->setEnabled(true) : ui->btn_Agregar->setEnabled(false);
}

void Product::on_btn_Agregar_clicked()
{
    emit addItem(idProduct, ui->sp_Cantidad->value());
    ui->sp_Cantidad->setValue(0);
}
