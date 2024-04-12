def run_to_converge(func):
    def wrapper(*argc, **kwargs) -> bool:
        updated = False
        while func(*argc, **kwargs):
            updated = True
        return updated

    return wrapper
