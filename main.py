import sys
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QVBoxLayout, QPushButton, QFileDialog, QComboBox, QTextEdit
import subprocess

class CipherApp(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        # Selector de archivos ejecutables
        self.executable_label = QLabel("Seleccionar ejecutable:")
        self.executable_combo = QComboBox()
        self.executable_combo.addItems(["md4.out", "md5.out", "sha1.out", "sha256.out", "hmac.out"])

        # Botón para seleccionar archivo de texto
        self.text_label = QLabel("Seleccionar archivo de texto:")
        self.text_button = QPushButton("Seleccionar archivo")
        self.text_button.clicked.connect(self.showDialog)

        # Etiqueta para mostrar la ruta del archivo de texto seleccionado
        self.text_path_label = QLabel()

        # Área de texto para mostrar el resultado del cifrado
        self.result_label = QLabel("Resultado del cifrado:")
        self.result_text = QTextEdit()
        self.result_text.setReadOnly(True)

        # Botón para realizar el cifrado
        self.encrypt_button = QPushButton("Cifrar")
        self.encrypt_button.clicked.connect(self.encryptText)

        # Diseño de la interfaz
        vbox = QVBoxLayout()
        vbox.addWidget(self.executable_label)
        vbox.addWidget(self.executable_combo)
        vbox.addWidget(self.text_label)
        vbox.addWidget(self.text_button)
        vbox.addWidget(self.text_path_label)
        vbox.addWidget(self.result_label)
        vbox.addWidget(self.result_text)
        vbox.addWidget(self.encrypt_button)

        self.setLayout(vbox)

        self.setGeometry(300, 300, 400, 400)
        self.setWindowTitle('Calculadora de Cifrado')
        self.show()

    def showDialog(self):
        fname = QFileDialog.getOpenFileName(self, 'Seleccionar archivo', './')

        if fname[0]:
            self.text_path_label.setText(fname[0])

    def encryptText(self):
        # Obtener la ruta del ejecutable seleccionado y del archivo de texto
        executable_path = "/home/alejandro/Documents/seguridad en computacion/lab6/" + self.executable_combo.currentText()  # Reemplazar con la ruta correcta
        text_path = self.text_path_label.text()

        # Ejecutar el cifrado y obtener el resultado
        result = subprocess.run([executable_path, text_path], capture_output=True, text=True)

        # Mostrar el resultado en el área de texto
        self.result_text.setPlainText(result.stdout)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = CipherApp()
    sys.exit(app.exec_())
