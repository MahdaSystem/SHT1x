idf_component_register(
  SRCS "main.c" "./main.c" "../../../../src/SHT1x.c" "../../../../port/ESP32-IDF/SHT1x_platform.c"
  INCLUDE_DIRS "../../../../src/include" "../../../../config" "../../../../port/ESP32-IDF"
  )