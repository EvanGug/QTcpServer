#pragma once

#include <QtWidgets/QWidget>
#include "ui_tcpserver.h"
#include <QTcpServer>
#include <QTcpSocket>

class TcpServer : public QWidget
{
	Q_OBJECT

public:
	TcpServer(QWidget *parent = Q_NULLPTR);
	~TcpServer();

private slots:
	void on_buttonListen_clicked();
	void on_buttonSend_clicked();

	void slotNewConnection();
	void slotTcpReceive();
	void slotDisconnect();

private:
	Ui::TcpServerClass *ui;

	QTcpSocket *m_tcpSocket;
	QTcpServer *m_tcpServer;
};
