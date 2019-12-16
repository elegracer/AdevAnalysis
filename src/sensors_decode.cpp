#include <cstdio>
#include <array>
#include <libsensors.h>
#include <imu_data.h>

ImuCsv acc_csv("acc");
ImuCsv gyr_csv("gyr");

class SensorsDecoder : public libsensors::Sensors {
  protected:
    void on_gyroscope(double t, double x, double y, double z) override {
        gyr_csv.append(t, x, y, z);
    }

    void on_accelerometer(double t, double x, double y, double z) override {
        acc_csv.append(t, x, y, z);
    }
};

int main() {
    SensorsDecoder decoder;
    std::array<unsigned char, 8192> buffer;
    while (true) {
        size_t nread = fread(buffer.data(), 1, buffer.size(), stdin);
        if (nread > 0) {
            decoder.parse_data(buffer.data(), nread);
        } else {
            if (feof(stdin) || ferror(stdin)) {
                break;
            }
        }
    }

    acc_csv.save("acc.csv");
    gyr_csv.save("gyr.csv");
    return 0;
}
