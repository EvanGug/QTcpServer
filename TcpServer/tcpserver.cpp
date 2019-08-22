#include "tcpserver.h"

TcpServer::TcpServer(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::TcpServerClass)
	, m_tcpServer(new QTcpServer(this))
	, m_tcpSocket(nullptr)
{
	ui->setupUi(this);

	ui->buttonSend->setEnabled(false);
	ui->lineEditPort->setText("1471");
	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

TcpServer::~TcpServer()
{
	delete ui;
}

void TcpServer::slotDisconnect()
{
	qInfo() << QString::fromLocal8Bit("断开连接");
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
	qInfo() << QString::fromLocal8Bit("新的TCP连接");
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
		ui->buttonListen->setText(QString::fromLocal8Bit("开启监听"));
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
			ui->buttonListen->setText(QString::fromLocal8Bit("关闭监听"));
			ui->buttonSend->setEnabled(true);
		}
	}
}

void TcpServer::on_buttonSend_clicked()
{
	m_tcpSocket->write(ui->textEditSend->toPlainText().toUtf8());
	m_tcpSocket->flush();
}