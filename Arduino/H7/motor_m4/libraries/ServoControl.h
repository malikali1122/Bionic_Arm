#ifndef _ServoControl_h
#define _ServoControl_h

#include <Arduino.h>
#include <Servo.h>

class ServoControl {
	private:
		Servo _servo;
		int _posAngle; // current position of servo wiper, between _lowerLim and _upperLim
		int _stepSize; // determines how many 'degrees' to move by per iteration
		int _upperLim; // upper servo limit (0-180), may differ between specific servo models
    int _upperPotFdbk; // pot feedback corresponding to upper lim
		int _lowerLim; // lower servo limit (0-180)
    int _lowerPotFdbk;
    float _fdbkGradient; // gradient of delta pos and delta fdbk; IDEALLY linear, i.e., fdbk = a*pos + b
		bool _holdPos; // flag to maintain current angle
    int _prevPos = 0; // pot feedback previous
    int _pos = 0; // pot feedback

	public:
		ServoControl(int stepSize, int upperLim = 180, int lowerLim = 0) {
			_posAngle = 0;
			_stepSize = stepSize;
			_upperLim = upperLim;
			_lowerLim = lowerLim;
			_holdPos = false;
		}

    attach(int pin) {
      _servo.attach(pin);
    }

    void moveTo(int pos) {
      _servo.write(pos);
    }

		void move(int posDir) {
      _prevPos = analogRead(A0);
			if (posDir)
			{
				_stepSize = abs(_stepSize);
			}
			else _stepSize = -1 * abs(_stepSize);
			
			if (!_holdPos) {
				_posAngle += _stepSize;
				if (_posAngle >= _upperLim) {
					_holdPos = true; // hit limit therefore hold position regardless of external force
					_posAngle = _upperLim; // saturate
				}
				else if (_posAngle <= _lowerLim) {
					_holdPos = true; // hit limit therefore hold position regardless of external force
					_posAngle = _lowerLim; // saturate
				}
				_servo.write(_posAngle);
        delay(100);
        _pos = analogRead(A0);
        _checkStalled();
			}
		}

		void stop() {
			// upon receiving stop signal, move once more to apply torque for holding then signify stall
			if (!_holdPos) {
				_posAngle += _stepSize;
        //_servo.write(_posAngle);
        delay(100);
				_holdPos = true;
			}
		}

		void releaseLock() {
			_holdPos = false;
		}

    void synchronize() {
      _servo.write(_lowerLim); // move to smallest possible position
      delay(500);
      int potFdbk = analogRead(A0); // save pot feedback at lowest position
      _lowerPotFdbk = potFdbk;

      _servo.write(_upperLim); // repeat for high limits
      delay(500);
      potFdbk = analogRead(A0);
      _upperPotFdbk = potFdbk;

      _servo.write(_lowerLim); // move back to lower limit
      delay(500);

      // calculating gradient of potentiometer feedback using linear interpolation
      _fdbkGradient = float(_upperPotFdbk - _lowerPotFdbk) / (_upperLim - _lowerLim);
    }

    bool isLocked() {
      return _holdPos; // return boolean flag to signify whether servo has locked/stalled
    }

    bool _checkStalled() {
      // checking stalling by seeing delta feedback and delta pos
      // if this is significantly lower than expected, then stalled
      Serial.print("Checking stall: ");
      Serial.println(abs(_pos - _prevPos)); // check delta in movement
      if (abs(_pos - _prevPos) < abs(0.6*_fdbkGradient*_stepSize)) { // must be parametrized in future; currently, <60% expected delta signifies stall
        stop();
        Serial.println("STALLED!!"); // testing purposes
      }
    }
  
};

#endif