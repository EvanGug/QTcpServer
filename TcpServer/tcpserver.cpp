#include "tcpserver.h"

TcpServer::TcpServer(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::TcpServerClass)
	, m_tcpServer(new QTcpServer(this))
	, m_tcpSocket(nullptr)
{
	ui->setupUi(this);

	ui->buttonSend->setEnabled(false);

	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

TcpServer::~TcpServer()
{
	delete ui;
}

void TcpServer::slotDisconnect(){
	if (this->sender() == m_tcpSocket)
	{
		m_tcpSocket = 0;
	}
}

void TcpServer::slotNewConnection()
{
	m_tcpSocket = m_tcpServer->nextPendingConnection();
	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(slotTcpReceive()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnect()));
	qInfo() << QString::fromLocal8Bit("�µ�TCP����");
}

void TcpServer::slotTcpReceive()
{
	QByteArray buffer;
	buffer = m_tcpSocket->readAll();
	QString tempStr = ui->textEditReceive->toPlainText();
	if (!tempStr.isEmpty())
	{
		tempStr += "\n";
	}
	tempStr += buffer;
	ui->textEditReceive->setText(tempStr);
}

void TcpServer::on_buttonListen_clicked()
{
	if (m_tcpServer->isListening())
	{
		m_tcpServer->close();
		ui->buttonListen->setText(QString::fromLocal8Bit("��������"));
		ui->buttonSend->setEnabled(false);
	}
	else
	{
		if (!m_tcpServer->listen(QHostAddress::Any, ui->lineEditPort->text().toInt()))
		{
			qDebug() << m_tcpServer->errorString();
			m_tcpServer->close();
		}
		else
		{
			ui->buttonListen->setText(QString::fromLocal8Bit("�رռ���"));
			ui->buttonSend->setEnabled(true);
		}
	}
}

void TcpServer::on_buttonSend_clicked()
{
	m_tcpSocket->write(ui->textEditSend->toPlainText().toUtf8());
	m_tcpSocket->flush();
}