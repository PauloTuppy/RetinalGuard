"""
RetinalGuard - Tuya Cloud AI Agent Integration
Generates human-readable medical reports from screening results
"""

import json
import os
from datetime import datetime
from typing import Dict, Any
import requests

class TuyaHealthAgent:
    """Wrapper for Tuya Cloud AI Agent"""
    
    def __init__(self, client_id: str, client_secret: str):
        self.client_id = client_id
        self.client_secret = client_secret
        self.access_token = None
        self.base_url = "https://api.tuya.com/v2.0"
        self.authenticate()
    
    def authenticate(self):
        """Authenticate with Tuya Cloud"""
        auth_url = f"{self.base_url}/auth/token"
        payload = {
            "grant_type": "1",
            "client_id": self.client_id,
            "client_secret": self.client_secret
        }
        response = requests.post(auth_url, json=payload)
        if response.status_code == 200:
            data = response.json()
            self.access_token = data['result']['access_token']
            print("✓ Authenticated with Tuya Cloud")
        else:
            raise Exception("Failed to authenticate with Tuya Cloud")
    
    def generate_report(self, screening_result: Dict[str, Any]) -> Dict:
        """
        Generate medical report from screening data
        
        Args:
            screening_result: {
                'patient_id': str,
                'cvd_risk_score': float (0-1),
                'heart_rate': int,
                'blood_oxygen': float,
                'temperature': float,
                'blood_pressure_systolic': float
            }
        
        Returns:
            Report dict with risk_level, recommendation, explanation, etc.
        """
        
        # Classify risk level
        risk_score = screening_result['cvd_risk_score']
        if risk_score > 0.7:
            risk_level = "HIGH"
            urgency = "URGENT"
        elif risk_score > 0.4:
            risk_level = "MODERATE"
            urgency = "STANDARD"
        else:
            risk_level = "LOW"
            urgency = "ROUTINE"
        
        # Generate recommendations
        recommendations = self._generate_recommendations(
            risk_level,
            screening_result
        )
        
        # Create report
        report = {
            "patient_id": screening_result['patient_id'],
            "screening_timestamp": datetime.now().isoformat(),
            "risk_level": risk_level,
            "cvd_risk_score": risk_score,
            "risk_percentage": f"{risk_score * 100:.1f}%",
            "urgency": urgency,
            "vital_signs": {
                "heart_rate_bpm": screening_result['heart_rate'],
                "blood_oxygen_percent": screening_result['blood_oxygen'],
                "temperature_celsius": screening_result['temperature'],
                "systolic_bp_mmhg": screening_result['blood_pressure_systolic']
            },
            "clinical_explanation": self._get_clinical_explanation(risk_level),
            "recommendations": recommendations,
            "followup_days": self._get_followup_days(risk_level)
        }
        
        return report
    
    def _generate_recommendations(self, risk_level: str, 
                                 screening_result: Dict) -> list:
        """Generate specific medical recommendations"""
        
        recommendations = []
        
        if risk_level == "HIGH":
            recommendations.extend([
                "Consult a cardiologist within 1 week",
                "Schedule ECG and echocardiography",
                "Reduce sodium intake to <2300mg/day",
                "Increase physical activity: 30min/day moderate exercise",
                "Monitor blood pressure daily",
                "Consider blood pressure medication if not on one"
            ])
        
        elif risk_level == "MODERATE":
            recommendations.extend([
                "Schedule cardiology appointment within 4 weeks",
                "Monitor blood pressure 2-3 times/week",
                "Maintain healthy diet (Mediterranean diet recommended)",
                "Exercise 30 minutes daily, 5 days/week",
                "Manage stress through meditation or yoga",
                "Avoid smoking and excessive alcohol"
            ])
        
        else:  # LOW
            recommendations.extend([
                "Maintain current lifestyle habits",
                "Continue regular exercise and healthy diet",
                "Recheck cardiovascular screening in 12 months",
                "Monitor blood pressure quarterly",
                "Annual health checkup recommended"
            ])
        
        # Add vital-sign specific advice
        if screening_result['heart_rate'] > 100:
            recommendations.append("Heart rate elevated; reduce caffeine and stress")
        
        if screening_result['temperature'] > 37.5:
            recommendations.append("Slight fever detected; consult physician")
        
        return recommendations
    
    def _get_clinical_explanation(self, risk_level: str) -> str:
        """Get clinical explanation for lay audience"""
        
        explanations = {
            "HIGH": (
                "Your retinal images show signs associated with cardiovascular "
                "disease risk. Blood vessels in the eye show changes that correlate "
                "with heart and blood pressure problems. This does not mean you "
                "definitely have heart disease, but it indicates a higher risk. "
                "We recommend urgent follow-up with a cardiologist for further testing."
            ),
            "MODERATE": (
                "Your screening shows moderate cardiovascular risk. While not urgent, "
                "we recommend lifestyle modifications and follow-up with a doctor "
                "within 4 weeks. Regular exercise, healthy diet, and stress management "
                "can significantly reduce your risk."
            ),
            "LOW": (
                "Your screening indicates low cardiovascular risk. Continue your "
                "current healthy lifestyle habits. Regular exercise, balanced diet, "
                "and annual health checkups help maintain good cardiovascular health."
            )
        }
        
        return explanations.get(risk_level, "")
    
    def _get_followup_days(self, risk_level: str) -> int:
        """Return recommended days until follow-up"""
        return {
            "HIGH": 7,
            "MODERATE": 28,
            "LOW": 365
        }.get(risk_level, 365)
    
    def send_to_dashboard(self, report: Dict) -> bool:
        """Send report to clinic dashboard"""
        
        endpoint = f"{self.base_url}/cloud/health/report"
        headers = {
            "Authorization": f"Bearer {self.access_token}",
            "Content-Type": "application/json"
        }
        
        response = requests.post(endpoint, json=report, headers=headers)
        return response.status_code == 200


# Example usage
if __name__ == "__main__":
    # Initialize agent
    agent = TuyaHealthAgent(
        client_id=os.getenv("TUYA_CLIENT_ID"),
        client_secret=os.getenv("TUYA_CLIENT_SECRET")
    )
    
    # Example screening result from T5 device
    screening_data = {
        "patient_id": "P001",
        "cvd_risk_score": 0.75,
        "heart_rate": 88,
        "blood_oxygen": 97.5,
        "temperature": 36.8,
        "blood_pressure_systolic": 145
    }
    
    # Generate report
    report = agent.generate_report(screening_data)
    
    # Pretty print
    print(json.dumps(report, indent=2))
    
    # Send to clinic dashboard
    agent.send_to_dashboard(report)
