#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Ventana principal
    QWidget window;
    window.setWindowTitle("Cifrador de Texto");

    // Diseño de la interfaz
    QVBoxLayout *layout = new QVBoxLayout(&window);

    // Selector de archivos ejecutables
    QLabel *executableLabel = new QLabel("Seleccionar ejecutable:");
    QComboBox *executableComboBox = new QComboBox();
    executableComboBox->addItem("Ejecutable 1");
    executableComboBox->addItem("Ejecutable 2");
    executableComboBox->addItem("Ejecutable 3");
    executableComboBox->addItem("Ejecutable 4");
    executableComboBox->addItem("Ejecutable 5");

    // Campo de texto para ingresar texto
    QLabel *textInputLabel = new QLabel("Ingresar texto:");
    QLineEdit *textInput = new QLineEdit();

    // Botón para seleccionar archivo de texto
    QPushButton *selectFileButton = new QPushButton("Seleccionar Archivo");
    QLineEdit *filePathInput = new QLineEdit();
    filePathInput->setReadOnly(true);

    // Conectar el botón para seleccionar archivo
    QObject::connect(selectFileButton, &QPushButton::clicked, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&window, "Seleccionar Archivo", "", "Archivos de Texto (*.txt)");
        filePathInput->setText(filePath);
    });

    // Botón para iniciar el cifrado
    QPushButton *encryptButton = new QPushButton("Cifrar");

    // Conectar el botón de cifrado (aquí deberías agregar la lógica de cifrado)
    QObject::connect(encryptButton, &QPushButton::clicked, [&]() {
        // Obtener la información seleccionada y realizar el cifrado
        QString selectedExecutable = executableComboBox->currentText();
        QString textToEncrypt = textInput->text();
        QString filePathToEncrypt = filePathInput->text();

        // Aquí puedes llamar a tu lógica de cifrado con la información proporcionada
        // ...

        // Imprimir información (esto debería ser reemplazado por la lógica de cifrado real)
        qDebug() << "Ejecutable seleccionado:" << selectedExecutable;
        qDebug() << "Texto a cifrar:" << textToEncrypt;
        qDebug() << "Archivo seleccionado:" << filePathToEncrypt;
    });

    // Agregar widgets al diseño
    layout->addWidget(executableLabel);
    layout->addWidget(executableComboBox);
    layout->addWidget(textInputLabel);
    layout->addWidget(textInput);
    layout->addWidget(selectFileButton);
    layout->addWidget(filePathInput);
    layout->addWidget(encryptButton);

    // Mostrar la ventana
    window.show();

    return app.exec();
}
