# IoT Lab – Field Mapping & Portal Reference

## Portal URL
https://careers.zelbytes.com → IoT Lab → Dashboards

## API Endpoint
POST https://careers.zelbytes.com/api/iot-lab/v1/telemetry

## Auth Header
X-Iot-Lab-Key: <your zil_... key from IoT Lab → My Instance>

## JSON Field Mapping
| Sensor Reading  | JSON Field      | Unit    |
|-----------------|-----------------|---------|
| DHT22 Temp      | temperature_c   | Celsius |
| DHT22 Humidity  | humidity_pct    | %       |
| Device identity | device_id       | string  |

## Device ID Namespace
Match your lab bench: e.g. bench_yourname_01

## Sample Interval
60 seconds (polyhouse bench policy)
