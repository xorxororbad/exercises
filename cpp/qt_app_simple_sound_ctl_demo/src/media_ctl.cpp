#include <QPointer>
#include <print>

#include "media_ctl.h"
#include "media_ctl_dbus.h"

QMediaCtl::QMediaCtl(QObject *parent)
    : QObject(parent)
    , m_status(QMediaCtl::Status::PAUSED)
    , m_impl(new MediaCtlDbus(
          [media_ctl = QPointer<QMediaCtl>(this)](bool playing)
          {
              if (media_ctl)
              {
                  QMetaObject::invokeMethod(
                      media_ctl,
                      [media_ctl, playing]()
                      {
                          if (media_ctl)
                          {
                              Status status = (playing ? QMediaCtl::Status::PLAY : QMediaCtl::Status::PAUSED);
                              media_ctl->set_status(status);
                          }
                      },
                      Qt::QueuedConnection);
              }
          }))
{
}

QMediaCtl::~QMediaCtl()
{
}

QMediaCtl::Status QMediaCtl::status() const
{
    return m_status;
}

void QMediaCtl::set_status(Status st)
{
    if (st != m_status)
    {
        m_status = st;
        emit status_changed(st);
    }
}

QMediaCtl::Status QMediaCtl::init()
{
    if (!m_impl->init())
    {
        set_status(QMediaCtl::Status::INIT_FAILED);
    }
    else
    {
        set_status(QMediaCtl::Status::PAUSED);
    }

    return m_status;
}

void QMediaCtl::next()
{
    if (m_status != QMediaCtl::Status::INIT_FAILED)
    {
        m_impl->next();
    }
}

void QMediaCtl::previous()
{
    if (m_status != QMediaCtl::Status::INIT_FAILED)
    {
        m_impl->previous();
    }
}

void QMediaCtl::toggle_play()
{
    if (m_status != QMediaCtl::Status::INIT_FAILED)
    {
        m_impl->toggle_play();
    }
}

