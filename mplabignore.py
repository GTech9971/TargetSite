def main():
    projectName = input("Enter mplab x project name:")
    with open(".gitignore", mode="a") as f:
        f.write(f"{projectName}/*\n")
        f.write(f"!{projectName}/*.c\n")
        f.write(f"!{projectName}/*.h\n")
        f.write(f"!{projectName}/Makefile\n")
        f.write(f"!{projectName}/nbproject\n")
        f.write(f"{projectName}/nbproject/*\n")
        f.write(f"!{projectName}/nbproject/configurations.xml\n")
        f.write(f"!{projectName}/nbproject/project.xml\n")
    print(f"Create {projectName} .gitignore")


if __name__ == '__main__':
    main()