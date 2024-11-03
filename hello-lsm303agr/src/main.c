#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int ax, ay, az, mx, my, mz;

// Change this
const int hard_iron[3] = {-144, 81, -241};

// Change this
const float soft_iron[3][3] = {
    {0.965, 0.010, 0.009}, 
	{0.010, 0.999, -0.021}, 
	{0.009, -0.021, 1.038}
};

// Change this
const double declination = 0.38;

static int32_t read_sensor(const struct device *sensor,
                           enum sensor_channel channel) {
  struct sensor_value val[3];
  int32_t ret = 0;

  ret = sensor_sample_fetch(sensor);
  if (ret < 0 && ret != -EBADMSG) {
    printf("Sensor sample update error\n");
    goto end;
  }

  ret = sensor_channel_get(sensor, channel, val);
  if (ret < 0) {
    printf("Cannot read sensor channels\n");
    goto end;
  }

  if (channel == SENSOR_CHAN_ACCEL_XYZ) {
    ax = (int)(sensor_value_to_double(&val[0]) * 1);
    ay = (int)(sensor_value_to_double(&val[1]) * 1);
    az = (int)(sensor_value_to_double(&val[2]) * 1);
  }

  if (channel == SENSOR_CHAN_MAGN_XYZ) {
    mx = (int)(sensor_value_to_double(&val[0]) * 1000);
    my = (int)(sensor_value_to_double(&val[1]) * 1000);
    mz = (int)(sensor_value_to_double(&val[2]) * 1000);
  }
end:
  return ret;
}

int main(void) {
  const struct device *const accelerometer = DEVICE_DT_GET_ONE(st_lis2dh);
  const struct device *const magnetometer =
      DEVICE_DT_GET_ONE(st_lsm303agr_magn);

  if (!device_is_ready(accelerometer)) {
    printf("Device %s is not ready\n", accelerometer->name);
    return 0;
  }

  if (!device_is_ready(magnetometer)) {
    printf("Device %s is not ready\n", magnetometer->name);
    return 0;
  }

  while (1) {
    if (read_sensor(magnetometer, SENSOR_CHAN_MAGN_XYZ) < 0) {
      printf("Failed to read magnetometer data\n");
    }

    if (read_sensor(accelerometer, SENSOR_CHAN_ACCEL_XYZ) < 0) {
      printf("Failed to read accelerometer data\n");
    }

	// Hard Iron Calibration
    int hi_cal[3];
    hi_cal[0] = mx - hard_iron[0];
    hi_cal[1] = my - hard_iron[1];
    hi_cal[2] = mz - hard_iron[2];

	// Soft Iron Calibration
    float mag_data[3];
    for (int i = 0; i < 3; i++) {
      mag_data[i] = (soft_iron[i][0] * hi_cal[0]) +
                    (soft_iron[i][1] * hi_cal[1]) +
                    (soft_iron[i][2] * hi_cal[2]);
    }

    printf("Raw:0,0,0,%d,%d,%d,%d,%d,%d\n", ax, ay, az, (int)mag_data[0],
           (int)mag_data[1], (int)mag_data[2]);

	// Declination
    double heading =
        -(atan2(mag_data[0], mag_data[1]) * 180) / M_PI - declination;
    if (heading < 0)
      heading = heading + 360;

    printf("Heading:%.2f\n", heading);

    k_sleep(K_MSEC(10));
  }
  return 0;
}
