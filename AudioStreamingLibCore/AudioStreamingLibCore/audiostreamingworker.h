#ifndef AUDIOSTREAMINGWORKER
#define AUDIOSTREAMINGWORKER

#include <QtCore>
#include "common.h"
#include "audiostreaminglibcore.h"
#include "discoverserver.h"
#include "server.h"
#include "client.h"
#include "encryptedserver.h"
#include "encryptedclient.h"
#include "webclient.h"
#include "audioinput.h"
#include "audiooutput.h"
#include "flowcontrol.h"
#include "levelmeter.h"
#ifdef OPUS
#include "opusdecoderclass.h"
#include "opusencoderclass.h"
#include "r8brain.h"
#endif

class AudioStreamingWorker : public QObject
{
    Q_OBJECT
public:
    explicit AudioStreamingWorker(QObject *parent = nullptr);
    ~AudioStreamingWorker();

signals:
    void connected(QHostAddress,QString);
    void connectedToServer(QByteArray);
    void disconnected(QHostAddress);
    void pending(QHostAddress,QString);
    void webClientLoggedIn();
    void webClientWarning(QString);
    void inputData(QByteArray);
    void veryInputData(QByteArray);
    void outputData(QByteArray);
    void veryOutputData(QByteArray);
    void extraData(QByteArray);
    void inputLevel(float);
    void outputLevel(float);
    void adjustSettings();
    void extraDataWritten();
    void commandXML(QByteArray);
    void error(QString);

public slots:
    void start(const StreamingInfo &streaming_info);
    void listen(quint16 port, bool auto_accept, const QByteArray &password, int max_connections);
    void connectToHost(const QString &host, quint16 port, const QByteArray &password);
    void writeCommandXML(const QByteArray &XML);
    void connectToPeer(const QString &ID);
    void disconnectFromPeer();
    void acceptConnection();
    void rejectConnection();
    void acceptSslCertificate();
    void writeExtraData(const QByteArray &data);
    void writeExtraDataResult();
    void inputDataBack(const QByteArray &data);
    void outputDataBack(const QByteArray &data);
    bool isInputMuted() const;
    void setInputMuted(bool mute);
    int volume() const;
    void setVolume(int volume);
    StreamingInfo streamingInfo() const;
    QList<QHostAddress> connectionsList() const;
    bool isReadyToWriteExtraData() const;

private slots:
    void startAudioWorkers();
    void stopAudioWorkers();
    void errorPrivate(const QString &error_description);
    void startOpusEncoder();
    void startOpusDecoder();
    void adjustSettingsPrivate(bool start_opus_encoder, bool start_opus_decoder, bool client_mode);
    void serverClientConencted(const PeerData &pd, const QString &id);
    void serverClientDisconencted(const PeerData &pd);
    void clientConencted(const PeerData &pd, const QString &id);
    void clientDisconencted(const PeerData &pd);
    void webClientConencted(const QByteArray &hash);
    void webClientConnectedToPeer(const QString &id);
    void webClientDisconnected();
    void posProcessedInput(const QByteArray &data);
    void preProcessOutput(const QByteArray &data);
    void posProcessedOutput(const QByteArray &data);
    void flowControl(int bytes);
    void processServerInput(const PeerData &pd);
    void processClientInput(const PeerData &pd);
    void processWebClientInput(const PeerData &pd);
    QByteArray createHeader();
    void header(QByteArray data,
                QAudioFormat *refInputAudioFormat,
                QAudioFormat *refAudioFormat,
                qint32 *refTimeToBuffer);

private:
    QPointer<DiscoverServer> m_server_discover;
    QPointer<AbstractServer> m_server;
    QPointer<AbstractClient> m_client;
    QPointer<AudioInput> m_audio_input;
    QPointer<AudioOutput> m_audio_output;
    QPointer<FlowControl> m_flow_control;
    QPointer<LevelMeter> m_level_meter_input;
    QPointer<LevelMeter> m_level_meter_output;
#ifdef OPUS
    QPointer<r8brain> m_resampler;
    QPointer<OpusEncoderClass> m_opus_enc;
    QPointer<OpusDecoderClass> m_opus_dec;
#endif
    bool m_input_muted;
    int m_volume;
    bool m_has_error;
    bool m_is_walkie_talkie;
    StreamingInfo m_streaming_info;
    bool m_ready_to_write_extra_data;
    int m_extra_data_peers;
    QList<qintptr> m_id_connections_list;
    QList<QHostAddress> m_host_connections_list;
    bool m_callback_enabled;

    QHash<qintptr, qint32> m_hash_heart_beat;
    QHash<qintptr, QElapsedTimer> m_hash_heart_beat_time;

#ifdef OPUS
    qint32 m_frame_size;
    qint32 m_max_frame_size;
    qint32 m_bitrate;
#endif
};

#endif // AUDIOSTREAMINGWORKER
