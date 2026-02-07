#ifndef MEDIA_CTL_H
#define MEDIA_CTL_H

#include <memory>

#include <QObject>

class MediaCtlDbus;

class QMediaCtl : public QObject
{
    Q_OBJECT


  public:
    enum Status
    {
        PLAY,
        PAUSED,
        INIT_FAILED,
    };
    Q_ENUM(Status)

    Q_PROPERTY(Status status READ status WRITE set_status NOTIFY status_changed)

    explicit QMediaCtl(QObject *parent = nullptr);
    virtual ~QMediaCtl();

    Status init();

    Q_INVOKABLE void next();
    Q_INVOKABLE void previous();
    Q_INVOKABLE void toggle_play();

    Status status() const;
    void set_status(Status st);

  signals:
    void status_changed(Status status);

  private:
    Status m_status;
    std::unique_ptr<MediaCtlDbus> m_impl;
};

#endif
