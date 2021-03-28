#ifndef PRODUCT_H
#define PRODUCT_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class Product;
}

class Product : public QWidget
{
    Q_OBJECT

public:
    explicit Product(QWidget *parent = nullptr);
    ~Product();

    QString getId() const;
    void setId(QString value);

    QString getName() const;
    void setName(QString value);

    void setImage();

    float getPriceProduct() const;
    void setPriceProduct(float value);

private slots:

    void on_sp_Cantidad_valueChanged(int arg1);

    void on_btn_Agregar_clicked();

signals:

    void addItem(QString id, int amount);

private:
    Ui::Product *ui;
    int peso;
    QString idProduct,nameProduct;
    float priceProduct;

};

#endif // PRODUCT_H
