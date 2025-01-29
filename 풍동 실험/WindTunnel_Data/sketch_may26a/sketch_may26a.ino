#include <HX711.h>
#define DOUT 3
#define CLK 2
HX711 scale(DOUT, CLK);

float calibration_factor = -208000; // 초기 보정 계수

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 Calibration");

  scale.set_scale(); // 초기화: 보정 계수 없이 설정
  scale.tare(); // 초기 무게를 0으로 설정
  long zero_factor = scale.read_average(); // 현재 무게 기준점 저장
  Serial.print("Zero factor: "); // 초기 기준점 출력
  Serial.println(zero_factor);
}

void loop() {
  scale.set_scale(calibration_factor); // 보정 계수 설정
  float weightInLbs = scale.get_units(); // 현재 무게를 파운드 단위로 읽기
  float weightInN = weightInLbs * 4.44822; // 파운드를 뉴턴으로 변환
  
  Serial.print("Drag Force(N): ");
  Serial.println(weightInN, 3); // 뉴턴 단위로 변환된 값 출력

  delay(1000); // 1초 대기

  // 시리얼 모니터를 통해 보정 계수를 조정
  if (Serial.available()) {
    char temp = Serial.read();
    if(temp == 'p') {
      calibration_factor += 1000; // '+' 키를 눌러 보정 계수 증가
      Serial.print("Calibration factor increased to: ");
      Serial.println(calibration_factor);
    }
    else if(temp == 'o') {
      calibration_factor -= 1000; // '-' 키를 눌러 보정 계수 감소
      Serial.print("Calibration factor decreased to: ");
      Serial.println(calibration_factor);
    }
    else if(temp == 's') {
      Serial.print("Calibration factor: ");
      Serial.println(calibration_factor); // 현재 보정 계수 출력
    }
    else if(temp == 'i') {
      scale.tare(); // 'i' 키를 눌러 초기화
      Serial.println("Scale reset to 0 N");
    }
  }
}
