#pragma once
#include<SimulateSet.h>

using namespace System;
using namespace Threading;

struct InputData
{
	double ats_start_x;
	double ats_start_y;
	double ats_target_x;
	double ats_target_y;
	bool attack_type;
	double ats_speed;

	double sms_x;
	double sms_y;
};

namespace ClassLibrary1 {

	public ref class OperatorController
	{
	public:
		void initOPC()
		{
			opc->initOperator();
		}

		void scenarioSetup(InputData inputs)
		{
			opc->setATSScenario(inputs.ats_start_x, inputs.ats_start_y, inputs.ats_target_x, inputs.ats_start_y, inputs.attack_type, inputs.ats_speed);
			opc->setSMSScenario(inputs.sms_x, inputs.sms_y);
		}
		void scenarioRelease()
		{
			opc->sendATSScenario();
			opc->sendSMSScenario();
		}
		void simulationStart()
		{
			opc->startSimulate();
			opc->threadPos();
		}
		void launch()
		{
			opc->launch();
		}

		int getATSStatus()
		{
			return opc->getATSStatus();
		}
		int getSMSStatus()
		{
			return opc->getSMSStatus();
		}
		double getATS_X()
		{
			return opc->getATSNowX();
		}
		double getATS_Y()
		{
			return opc->getATSNowY();
		}
		double getSMS_X()
		{
			return opc->getSMSNowX();
		}
		double getSMS_Y()
		{
			return opc->getSMSNowY();
		}

	private:
		SimulateSet* opc;
	};
}
