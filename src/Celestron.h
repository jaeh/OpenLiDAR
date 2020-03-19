#pragma once

#include <string>

typedef enum { GPS_OFF, GPS_ON } CELESTRON_GPS_STATUS;
typedef enum { SR_1, SR_2, SR_3, SR_4, SR_5, SR_6, SR_7, SR_8, SR_9 } CELESTRON_SLEW_RATE;
typedef enum { TRACKING_OFF, TRACK_ALTAZ, TRACK_EQN, TRACK_EQS } CELESTRON_TRACK_MODE;
typedef enum { RA_AXIS, DEC_AXIS } CELESTRON_AXIS;
typedef enum { CELESTRON_N, CELESTRON_S, CELESTRON_W, CELESTRON_E } CELESTRON_DIRECTION;
typedef enum { FW_MODEL, FW_VERSION, FW_GPS, FW_RA, FW_DEC } CELESTRON_FIRMWARE;

typedef struct {
    std::string Model;
    std::string Version;
    std::string RAFirmware;
    std::string DEFirmware;
    float controllerVersion;
    char controllerVariant;
    bool isGem;
} FirmwareInfo;

class Celestron {
public:
    Celestron();
    virtual ~Celestron();

    bool        connect(const std::string& _portName = "/dev/ttyUSB0");
    void        disconnect();

    bool        echo();
    bool        checkConnection();
    bool        checkAligned();
    bool        hibernate();
    bool        wakeup();
    bool        sync(double _ra, double _dec);

    bool        move(CELESTRON_DIRECTION _dir, CELESTRON_SLEW_RATE _rate);
    bool        stop(CELESTRON_DIRECTION _dir);
    bool        abort();

    bool        getFirmware(FirmwareInfo* _info);
    bool        getVersion(char* _version, int _size);
    bool        getVariant(char* _variant);
    bool        getModel(char* _model, int _size, bool* _isGem);
    bool        getDevFirmware(int _dev, char* _version, int _size);

    // Time & Location
    bool        setLocation(double _longitude, double _latitude);
    // bool        setDatetime(struct ln_date* _utc, double _utc_offset);

    bool        getRADec(double* _ra, double* _dec);
    bool        getAzAlt(double* _az, double* _alt);

    bool        slewRADec(double _ra, double _dec);
    bool        slewAzAlt(double _az, double _alt);
    bool        isSlewing();

    // Track Mode
    bool        getTrackMode(CELESTRON_TRACK_MODE *_mode);
    bool        setTrackMode(CELESTRON_TRACK_MODE _mode);

    // Pulse Guide (experimental)
    int         sendPulse(CELESTRON_DIRECTION _dir, signed char _rate, unsigned char _duration_msec);
    int         getPulseStatus(CELESTRON_DIRECTION _dir, bool &_pulse_state);

private:
    int     send_command(char *_cmd, int _cmd_len, char *_resp, int _resp_len);
    int     send_passthrough(int _dest, int _cmd_id, const char *_payload, int _payload_len, char *_response, int _response_len);

    int     m_slewRate;     /* Rate for slew request in StartSlew */
    int     m_fd = 0;

    bool    m_connected;
};
