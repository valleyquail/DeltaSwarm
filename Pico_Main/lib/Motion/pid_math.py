import numpy as np
import matplotlib.pyplot as plt
###
# Some rough approximations of the PID tuning of the robot


wheel_diameter = 0.0325  # meters
encoder_counts_per_rev = 12
gear_ratio = 70

def calc_encoder_speed(speed_ms: float, wheel_diameter: float, gear_ratio: int) -> float:
    wheel_circumference = np.pi * wheel_diameter
    wheel_speed = speed_ms / wheel_circumference
    encoder_speed = wheel_speed * encoder_counts_per_rev * gear_ratio
    return encoder_speed

#approximating the pid since the encoder counts need to be mapped to a float for
#duty cycles
# def approximate_pid(timer_ms: int, speed_ms: float) -> tuple:
#     encoder_speed = calc_encoder_speed(speed_ms, wheel_diameter, gear_ratio)
#     encoder_per_interval = encoder_speed * timer_ms/1000
#     kp = 0.1
#     ki = 0.01
#     kd = 0.01
#     #assume full error on p
    
#     real_speed = 0
    
#     integral_error = 0
#     derivative_error = 0
    
#     # plot the PID values
#     p_values = []
#     i_values = []
#     d_values = []
#     pid_speeds = []
#     sim_time = [0]
#     # encoder speed tolerance within +/- 10
#     error_range = 10
    
#     while real_speed > encoder_per_interval + error_range or real_speed < encoder_per_interval - error_range:
        
#         #approx the response of KP and then add some random variance to mimic real world
#         p_error = kp*(encoder_per_interval- real_speed)*np.random.uniform(0.7, 0.9)
#         p_values.append(p_error)
        
#         i_error = ki*integral_error*np.random.uniform(0.7, 0.9)
        
#         integral_error += i_error
#         i_values.append(i_error)
        
#         d_error = kd*derivative_error*np.random.uniform(0.7, 0.9)
#         d_values.append(d_error)
        
#         real_speed += p_error + i_error + d_error
#         pid_speeds.append(real_speed)

#         #keep the integral error from immediately spiking to max value
#         if encoder_speed - real_speed < 0.5 * encoder_speed:
#             integral_error += encoder_speed - real_speed
        
#         derivative_error = encoder_speed - real_speed

#         sim_time.append(timer_ms/1000 + sim_time[-1])
    
#     return p_values, i_values, d_values, pid_speeds, sim_time


import numpy as np

def approximate_pid(timer_ms: int, speed_ms: float, wheel_diameter: float, gear_ratio: float) -> tuple:
    def calc_encoder_speed(speed_ms, wheel_diameter, gear_ratio):
        # Calculate the encoder speed based on wheel diameter and gear ratio
        wheel_circumference = np.pi * wheel_diameter
        rpm = (speed_ms / wheel_circumference) * 60
        encoder_speed = rpm * gear_ratio
        return encoder_speed

    encoder_speed = calc_encoder_speed(speed_ms, wheel_diameter, gear_ratio)
    encoder_per_interval = encoder_speed * timer_ms / 1000
    
    # PID constants
    kp = 0.2
    ki = 0.1
    kd = 0.2
    
    # Simulation constants
    max_pwm = 100  # Maximum PWM duty cycle
    motor_inertia = 0.1  # Inertia factor for motor response
    friction = 0.05 # Friction damping factor
    noise_factor = 0.01  # Sensor noise magnitude
    
    # Initialize variables
    real_speed = 0
    integral_error = 0
    derivative_error = 0
    
    # Plot values
    p_values = []
    i_values = []
    d_values = []
    pid_speeds = []
    sim_time = []
    
    # Encoder speed tolerance within +/- 10
    error_range = 10

    while abs(real_speed - encoder_per_interval) > error_range:
        # Calculate PID errors
        error = encoder_per_interval - real_speed
        p_error = kp * error
        integral_error += error * timer_ms / 1000
        integral_error = np.clip(integral_error, -max_pwm / ki, max_pwm / ki)  # Anti-windup
        i_error = ki * integral_error
        d_error = kd * (error - derivative_error) / (timer_ms / 1000)
        derivative_error = error
        
        # Calculate PID output
        pid_output = p_error + i_error + d_error
        pid_output = np.clip(pid_output, -max_pwm, max_pwm)  # Saturation limit
        
        # Update motor speed with response time, friction, and noise
        motor_response = pid_output * motor_inertia
        real_speed += motor_response - (friction * real_speed)
        real_speed += np.random.uniform(-noise_factor, noise_factor) * encoder_per_interval
        
        # Log values
        p_values.append(p_error)
        i_values.append(i_error)
        d_values.append(d_error)
        pid_speeds.append(real_speed)
        sim_time.append(timer_ms / 1000 + sim_time[-1] if sim_time else timer_ms / 1000)

    return p_values, i_values, d_values, pid_speeds, sim_time




if __name__ == "__main__":
    speed = 1 #m/s
    p_vals, i_vals, d_vals, pid_speeds, sim_time = approximate_pid(20, speed, wheel_diameter, gear_ratio)
    
    plt.plot(sim_time, p_vals, label="P")
    plt.plot(sim_time, i_vals, label="I")
    plt.plot(sim_time, d_vals, label="D")
    plt.plot(sim_time, pid_speeds, label="PID")
    plt.legend()
    plt.show()
    
    
    