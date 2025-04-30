import subprocess

def check_code_format():
    subprocess.run(["treefmt"], check=True)
    subprocess.run(["git", "diff", "--exit-code"], check=True)

if __name__ == "__main__":
    check_code_format()